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