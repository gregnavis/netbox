netbox - simple packet analysis tools
=====================================

[![Build Status](https://travis-ci.org/grn/netbox.png?branch=master)](https://travis-ci.org/grn/netbox)

netbox is a set of simple programs for inspecting the network traffic at the
data link layer, network layer and transport layer. Its main design goal is
simplicity and lack of dependencies on external libraries, so it can be used
to debug networking problems by programmers and administrators alike.

Dumb usage
----------

First use `nbeth` to dump traffic from all network interfaces. It prints
captured packets in hexadecimal form to the standard output, one packet per
line.

For example:

```
# nbdump > dump
```

will dump network traffic to the file `dump`. All lines are of the form:

```
eth:<packet data>
```

The prefix `eth:` means that they are Ethernet packets and should be processed
with `nbeth`. In order to process them do the following:

```
$ cat dump | nbeth
```

An example line of output may look like:

```
eth.dst 38:8f:ac:19:0e:fa eth.src 29:fc:e0:01:8e:cc eth.proto 2048 ip4 \
    ip4:<packet data>
```

The fields `eth.dst` and `eth.src` are destination and source MAC addresses.
The field `eth.proto` is a protocol number. In the example above it equals to
2048 which is the IPv4. This is indicated by an additional symbolic value
`ip4` printed after `2048`.

In order to process the IPv4 run:

```
$ cat dump | nbeth | nbip4
```

This prints:

```
eth.dst 38:8f:ac:19:0e:fa eth.src 29:fc:e0:01:8e:cc eth.proto 2048 ip4 \
    ip.version 4 ip.ihl 5 ip.tos 00 ip.length 13312 ip.id 29613 \
    ip.flags 0dm ip.offset 64 ip.ttl 64 ip.protocol 6 tcp \
    ip.checksum 8877 ip.src 192.168.1.2 ip.dst 192.168.1.1 \
    tcp:<packet data>
```

The fields prefixed with `ip.` are the IPv4 headers. Note that the TOS and the
checksum are hexadecmial numbers. The field `ip.flags` require a little
explanation: `0` is the value of the reserved flags (which should be zero),
`d` means that the don't fragment flag is not set, and `m` means that the more
fragments flag is not set. If a flag is set then a capital letter is printed.

The `tcp:` prefix before the packet data means that the frame contains a TCP
packet and should be processed by `nbtcp`.

```
eth.dst 38:8f:ac:19:0e:fa eth.src 29:fc:e0:01:8e:cc eth.proto 2048 ip4 \
    ip.version 4 ip.ihl 5 ip.tos 00 ip.length 13312 ip.id 29613 \
    ip.flags 0dm ip.offset 64 ip.ttl 64 ip.protocol 6 tcp \
    ip.checksum 8877 ip.src 192.168.1.2 ip.dst 192.168.1.1 \
    tcp.src 51369 tcp.dst 20480 tcp.seq 3908834420 tcp.ack 3871014084 \
    tcp.offset 8 tcp.flags uAprsf tcp.window 31488 tcp.check ef15 \
    tcp.urg 0 unknown:<packet data>
```

The TCP flags have interpretation similar to IPv4 flags. Lower case letters
mean that the flag is not set and capital letters mean that the flag is set.
The TCP checksum in `tcp.check` is a hexadecimal number.

The `unknown:` prefix means that the packet data format is unknown to netbox
and cannot be processed further.

Smart usage
-----------

The usage above has two major drawbacks:

- it requires building a complete command step-by-step because you have to
  identify the protocol that should be dumped
- it doesn't work for dumps that contain different packet structures. For
  example if a dump contains a DNS query (Ethernet + IPv4 + UDP) and a TCP
  packet (Ethernet + IPv4 + TCP) then no command of the presented form will
  dump both of them.

To solve these problems just run:

```
$ cat dump | netbox
```

It will build appriopriate pipes on the fly by using prefixes of packet data.
The prefix `eth:` corresponds to `nbeth`, `ip4:` to `nbip4` and so on.

Supported protocols
-------------------

The following protocols are supported:

- Ethernet (`nbeth`)
- ARP (`nbarp`)
- IPv4 (`nbip4`)
- TCP (`nbtcp`)
- UDP (`nbudp`)

Limitations
-----------

- no support for IPv4/TCP options
- no support for IPv6, ICMP, IPsec
- lack of testing on systems other than Linux (probably it won't even
  compile!)
- lack of testing (so certainly there are bugs)
