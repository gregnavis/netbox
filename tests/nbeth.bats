#!/usr/bin/env bats

load test_helper

function run_nbeth() {
    run bash -c "echo $1\n | ./nbeth"
}

@test 'dump an Ethernet frame' {
    run_nbeth 'eth:010203040506111213141516080000'
    [ "$status" -eq 0 ]
    [ "$output" = 'eth.dst 01:02:03:04:05:06 eth.src 11:12:13:14:15:16 eth.proto 2048 ip4 ip4:00' ]
}

@test 'complain about empty Ethernet frames' {
    run_nbeth 'eth:'
    [ "$status" -eq 1 ]
    [ "$output" = 'Fatal: no data' ]
}

@test 'complain about lack of a protocol specifier' {
    run_nbeth ''
    [ "$status" -eq 1 ]
    [ "$output" = 'Fatal: expected a protocol specifier' ]
}

@test 'complain about too big Ethernet frames' {
    run_nbeth "eth:$(repeat_string 3000 a)"
    [ "$status" -eq 1 ]
    [ "$output" = 'Fatal: the maximum number of 2048 packet bytes is exceeded' ]
}
