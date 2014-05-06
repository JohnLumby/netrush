netrush
=======

utility for exchanging and logging messages between two machines

 netrush is a utility for exchanging and logging messages between two machines,
     enquirer and responder, over a network

    invoke as
     netrush [ -c | -s ] <enquirer:responder>|<enquirer,responder> [ -I <local_net_intrfc> ]
             -C <enquirerproccount> -S <responderproccount> -p <lowestport>
      [-m <max_sendcount> | -l <time_limit> ] [ -r <report_dir> ] [-n <sosndbufsz>] [-e <sorcvbufsz>]
      [-R] [-N] [-V] [-W] [-h] [-v] [-q] [-d <jobname>] [-w] [-t] [-i] [-k <job_identifier>]
      [ -o <binary_output_file> | -O <linefeed_delimited_output_file> ]
    exactly one of the following must be specified:
        -c = enquirer     -s = responder
    followed by <enquirer:responder> in the form enquirername:respondername
                where each name is a valid internet name or IPV4 numeric address that must not contain a : character
             or <enquirer,responder> in the form enquirername,respondername
                (the syntax with comma separator is required for IPV6 numeric addresses)
    where each name is a valid internet name or IPV6 address

    responder accepts messages from each of <enquirerproccount> connections
     and may respond in either of both of the following ways:
       .  send message back on the connection
       .  write contents of message to a binary or ascii file
    It permits the enquirers to disconnect and reconnect and will not terminate until explicitly told to.

    This utility may also play the role of enquirer by invoking with -c flag and other similar parameters.

   -I <local_net_intrfc> applies only when enquirer and responder IP addresses are in IPV6 form:
                         it specifies the local interface to be used for connecting.
                         Note that this may also be specified on each IPV6 address itself as a scope suffix
                              e.g. -c ff80::..,ff80::...%eth1
                              scope suffix, like local_net_intrfc, names the *local* interface, even for the remote IP address
                              Either <local_net_intrfc> or scope suffix *must* be specified for IPV6.

   in order to force use of IPV6,  you must either specify numeric IPV6 IP addresses
                                            or specify the -I flag
                                            or use the comma separator form of the <enquirer,responder> names.

    <enquirerproccount> specifies the number of worker processes on the enquirer machine
    <responderproccount> specifies the number of worker processes on the responder machine

    <lowestport>   -  lowest port number to be used.   each connection will use a pair of port numbers
                        each of which is a unique value added to <lowestport>
    <max_sendcount>        -  number of messages to be sent by each worker enquirer before ending job
 OR
    <time_limit>[,<nintervals>[,<nfyintvlnum>,<nfyintvlip>,<nfyintvlport>]]     -
                        the job is to finish after this number of seconds -
                        limits time that the process will run to <time_limit> seconds
                        nintervals requests that interim statistical reports be issued for each of nintervals intervals
                        nfyintvlnum,nfyintvlip,nfyintvlport requests to notify another process
                        by sending 5 udp messages to the specified address after end of interval nfyintvlnum
                        address family of nfyintvlip ( IPV4 or IPV6 ) *must* match that of the enquirer and responder names
                        If nfyintvlip is IPV6 and either the -I <local_net_intrfc> is omitted or the local interface
                        to be used is different,  then specify the interface as a scope suffix on nfyintvlip.
 OR   both max_sendcount and time-limit  -  the job finishes whenever the first occurs
    if   <report_dir>   is specified, each process will write a report to file <report_dir>/pid
    <sosndbufsz>          socket SO_SNDBUF option
    <sorcvbufsz>         socket SO_RCVBUF option

    -R                  tcp/ip RFC1323 option
    -N                  tcp/ip NODELAY option
    -W                  MSG_WAITALL option on recv

    -v verbose  (specify twice for maximum verbosity)
    -w suppress printing initially
    -t tabular output from supervisor
    -i timestamps initially
    <jobname>         -  if specified, is included in the name of every report file -
                        limited to 16 characters - suggestion is to provide todays date in short format
    <job_identifier>  assign each concurrent job a number unique in range 1-255
                       and specify this number on both enquirer and responder executions of that job
    <binary_output_file> or <linefeed_delimited_output_file> apply only to the responder
     and specify to write received messages to the named file - specify one or neither.
     For the binary_output_file,  messages from all enquirers are interleaved at byte granularity on tcp recv boundaries,
     which can occur anywhere within the stream and not under the control of sender or receiver;
     for the linefeed_delimited_output_file,  messages from all enquirers are interleaved at line granularity,
     that is,  lines delimited by linefeed will be preserved as contiguous sequences of characters.
