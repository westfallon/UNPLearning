clean:
	rm -rf target

run_main:
	mkdir -p target \
	&& gcc main.c -lunp -o target/main \
	&& ./target/main

run_daytime_srv:
	mkdir -p target/c4 \
	&& gcc c4/daytime_tcp_srv.c -o target/c4/daytime_srv -lunp \
	&& ./target/c4/daytime_srv

run_daytime_cli:
	mkdir -p target/c4 \
	&& gcc c4/daytime_tcp_cli.c -o target/c4/daytime_cli -lunp \
	&& ./target/c4/daytime_cli 127.0.0.1

run_echo_srv:
	mkdir -p target/c5 \
	&& gcc c5/echo_tcp_srv.c -o target/c5/echo_srv -lunp \
	&& ./target/c5/echo_srv

run_echo_cli:
	mkdir -p target/c5 \
	&& gcc c5/echo_tcp_cli.c -o target/c5/echo_cli -lunp \
	&& ./target/c5/echo_cli 127.0.0.1

run_select_echo_cli:
	mkdir -p target/c6 \
	&& gcc c6/echo_tcp_cli_select.c -o target/c6/select_echo_cli -lunp \
	&& ./target/c6/select_echo_cli 127.0.0.1

run_select_echo_srv:
	mkdir -p target/c6 \
	&& gcc c6/echo_tcp_srv_select.c -o target/c6/select_echo_srv -lunp \
	&& ./target/c6/select_echo_srv

run_poll_echo_srv:
	mkdir -p target/c6 \
	&& gcc c6/echo_tcp_srv_poll.c -o target/c6/poll_echo_srv -lunp \
	&& ./target/c6/poll_echo_srv

run_udp_echo_srv:
	mkdir -p target/c8 \
	&& gcc c8/echo_udp_srv.c -o target/c8/udp_echo_srv -lunp \
	&& ./target/c8/udp_echo_srv

run_udp_echo_cli:
	mkdir -p target/c8 \
	&& gcc c8/echo_udp_cli.c -o target/c8/udp_echo_cli -lunp \
	&& ./target/c8/udp_echo_cli 127.0.0.1

run_echo_srv_new:
	mkdir -p target/c8 \
	&& gcc c8/echo_srv.c -o target/c8/echo_srv -lunp \
	&& ./target/c8/echo_srv