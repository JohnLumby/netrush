/******************************************************************************
**
**    Copyright (C) 2011 , 2014    John Lumby    johnlumby@hotmail.com
**
*--------------------------------------------------------------------------*
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  The GNU General Public License, version 2, is viewable at
**              http://www.gnu.org/licenses/gpl-2.0.html
**  The following is a brief summary of specific points from the license
**  that apply to use of this program,  but not a replacement of that license.
**  Permission is granted to use this program for any legal purpose and to copy,
**  modify, or redistribute the program subject to the terms and restrictions
**  stated in the GNU General Public License, version 2
**  and specifically provided that you abide by both of the following
**  as well as that license :
**   .   You may not incorporate this program into a proprietary program either in part
**       or in whole,  either in source or compiled form,   proprietary meaning
**       either that you claim ownership of the program or that you sell it
**       for a fee or both.
**   .   You must retain the above copyright statement and this entire notice
**       in any copy or redistribution,  and make it clear that the licensing terms,
**       restrictions and obligations apply to whomever you give or redistribute
**       this program to, including this requirement to pass on the same terms,
**       restrictions and obligations to that recipient.
*--------------------------------------------------------------------------*
*/
char usage_text[] = " netrush : utility for exchanging and logging messages between two machines,\n"
"     enquirer and responder, over a network\n"
"\n"
"    invoke as\n"
"     %s [ -c | -s ] <enquirer:responder>|<enquirer,responder> [ -I <local_net_intrfc> ]\n"
"             -C <enquirerproccount> -S <responderproccount> -p <lowestport>\n"
"      [-m <max_sendcount> | -l <time_limit> ] [ -r <report_dir> ] [-n <sosndbufsz>] [-e <sorcvbufsz>]\n"
"      [-R] [-N] [-V] [-W] [-h] [-v] [-q] [-d <jobname>] [-w] [-t] [-i] [-k <job_identifier>]\n"
"      [ -o <binary_output_file> | -O <linefeed_delimited_output_file> ]\n"
"    exactly one of the following must be specified:\n"
"        -c = enquirer     -s = responder\n"
"    followed by <enquirer:responder> in the form enquirername:respondername\n"
"                where each name is a valid internet name or IPV4 numeric address that must not contain a : character\n"
"             or <enquirer,responder> in the form enquirername,respondername\n"
"                (the syntax with comma separator is required for IPV6 numeric addresses)\n"
"    where each name is a valid internet name or IPV6 address\n"
"\n"
"    responder accepts messages from each of <enquirerproccount> connections\n"
"     and may respond in either of both of the following ways:\n"
"       .  send message back on the connection\n"
"       .  write contents of message to a binary or ascii file\n"
"    It permits the enquirers to disconnect and reconnect and will not terminate until explicitly told to.\n"
"\n"
"    This utility may also play the role of enquirer by invoking with -c flag and other similar parameters.\n"
"\n"
"   -I <local_net_intrfc> applies only when enquirer and responder IP addresses are in IPV6 form:\n"
"                         it specifies the local interface to be used for connecting.\n"
"                         Note that this may also be specified on each IPV6 address itself as a scope suffix\n"
"                              e.g. -c ff80::..,ff80::...%%eth1\n"
"                              scope suffix, like local_net_intrfc, names the *local* interface, even for the remote IP address\n"
"                              Either <local_net_intrfc> or scope suffix *must* be specified for IPV6.\n"
"\n"
"   in order to force use of IPV6,  you must either specify numeric IPV6 IP addresses\n"
"                                            or specify the -I flag\n"
"                                            or use the comma separator form of the <enquirer,responder> names.\n"
"\n"
"    <enquirerproccount> specifies the number of worker processes on the enquirer machine\n"
"    <responderproccount> specifies the number of worker processes on the responder machine\n"
"\n"
"    <lowestport>   -  lowest port number to be used.   each connection will use a pair of port numbers\n"
"                        each of which is a unique value added to <lowestport>\n"
"    <max_sendcount>        -  number of messages to be sent by each worker enquirer before ending job\n"
" OR\n"
"    <time_limit>[,<nintervals>[,<nfyintvlnum>,<nfyintvlip>,<nfyintvlport>]]     -\n"
"                        the job is to finish after this number of seconds -\n"
"                        limits time that the process will run to <time_limit> seconds\n"
"                        nintervals requests that interim statistical reports be issued for each of nintervals intervals\n"
"                        nfyintvlnum,nfyintvlip,nfyintvlport requests to notify another process\n"
"                        by sending 5 udp messages to the specified address after end of interval nfyintvlnum\n"
"                        address family of nfyintvlip ( IPV4 or IPV6 ) *must* match that of the enquirer and responder names\n"
"                        If nfyintvlip is IPV6 and either the -I <local_net_intrfc> is omitted or the local interface\n"
"                        to be used is different,  then specify the interface as a scope suffix on nfyintvlip.\n"
" OR   both max_sendcount and time-limit  -  the job finishes whenever the first occurs\n"
"    if   <report_dir>   is specified, each process will write a report to file <report_dir>/pid\n"
"    <sosndbufsz>          socket SO_SNDBUF option\n"
"    <sorcvbufsz>         socket SO_RCVBUF option\n"
"\n"
"    -R                  tcp/ip RFC1323 option\n"
"    -N                  tcp/ip NODELAY option\n"
"    -W                  MSG_WAITALL option on recv\n"
"\n"
"    -v verbose  (specify twice for maximum verbosity)\n"
"    -w suppress printing initially\n"
"    -t tabular output from supervisor\n"
"    -i timestamps initially\n"
"    <jobname>         -  if specified, is included in the name of every report file -\n"
"                        limited to 16 characters - suggestion is to provide todays date in short format\n"
"    <job_identifier>  assign each concurrent job a number unique in range 1-255\n"
"                       and specify this number on both enquirer and responder executions of that job\n"
"    <binary_output_file> or <linefeed_delimited_output_file> apply only to the responder\n"
"     and specify to write received messages to the named file - specify one or neither.\n"
"     For the binary_output_file,  messages from all enquirers are interleaved at byte granularity on tcp recv boundaries,\n"
"     which can occur anywhere within the stream and not under the control of sender or receiver;\n"
"     for the linefeed_delimited_output_file,  messages from all enquirers are interleaved at line granularity,\n"
"     that is,  lines delimited by linefeed will be preserved as contiguous sequences of characters.\n"
;
           /*       J Lumby johnlumby@hotmail.com    */
#ifndef XSEND_LIMIT
#define XSEND_LIMIT 24  /*  enquirer : limit of excess of (sendcount - msgincount) on any connection     */
                        /*  values <= 0 have special significance:
                        **    0 <=>   no send limit - unrestricted  (same as defining NO_CLIENT_PACING)
                        **   -1 <=>   no send limit - unrestricted AND responder will not send replies (unidrectional traffic)
                        */
#endif

#if (XSEND_LIMIT < 1)
#define NO_CLIENT_PACING
#endif  /*  (XSEND_LIMIT < 1)  */
int set_rc_to_77(void);

#define NETRUSH_VERSION 2.0
#include <stdio.h>

#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <sys/ioctl.h>
const char message_som   = '\x11'; /* start-of-msg indicator  */
const char message_eom   = '\xee'; /* end-of-msg indicator  */
const char stream_eof[2] = "\xef"; /* end-of-stream indicator */
#define MESSAGE_BANNER_SIZE 11  /*  length of string - see below */
#define MESSAGE_SOMBAN_SIZE (MESSAGE_BANNER_SIZE+1)  /*  length of som indicator followed by banner string */
#define MESSAGE_HDR_SIZE (MESSAGE_SOMBAN_SIZE+11)    /*  length of som indicator followed by banner followed by pid */
#define MESSAGE_NUMWRITN_OFFSET (MESSAGE_HDR_SIZE+17)    /*  length of som indicator followed by banner followed by pid */
const char enquirer_message_banner[MESSAGE_BANNER_SIZE+1] = { 'c', 'l', 'i', 'e', 'n', 't', ' ', 'm', 's', 'g', ' ', '\0' };
const char responder_message_banner[MESSAGE_BANNER_SIZE+1] = { 's', 'e', 'r', 'v', 'e', 'r', ' ', 'm', 's', 'g', ' ', '\0' };

#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netdb.h>

struct sockaddr_general /*  my own version of a generalized sockaddr struct including a directly referencable port num */
  {
    union {                /*   union  that is at least as large as a sockaddr_in6 */
        struct sockaddr     opaque_sockaddr;  /*  semi-opaque - presumed to contain at least a
                                               *  directly referencable field named sa_family
                                               *  whose offset within each struct is the same (checked at runtime)
                                               */
        struct sockaddr_in    inet_sockaddr;  /*     pad it out to maximum length            */
        struct sockaddr_in6  inet6_sockaddr;  /*     pad it out to maximum length            */
    } sockaddr_u;
    /*************
    sa_family_t sin_family;
    *************/
    in_port_t sin_port;               /*  separate directly-referencable copy of portnum
                                       *  (i.e. ,  can be referenced without needing to test inet family)
                                       *   since we refer to it so often
                                       */
  };

#define SOCKADDR_SIZE ( (sizeof(struct sockaddr_general) + 7) & 0xfffffff8 )
/*   given a pointer to a sockaddr_general struct and a numeric port number,
 *   set the port into the two fields in the struct,  depending on inet family
 */
#define SET_PORTS_GENERAL(_sock_g_p_ , _portnum_) \
    (_sock_g_p_)->sin_port = htons(_portnum_);                 \
    if ( (_sock_g_p_)->sockaddr_u.opaque_sockaddr.sa_family == AF_INET6 ) \
        ((struct sockaddr_in6 *)(_sock_g_p_))->sin6_port = (_sock_g_p_)->sin_port; \
    else \
        ((struct sockaddr_in *)(_sock_g_p_))->sin_port = (_sock_g_p_)->sin_port;

/*   reconcile our copy of port number with what is in the sockaddr  */
#define RECONCILE_PORTS_GENERAL(_sock_g_p_ ) \
    if ( (_sock_g_p_)->sockaddr_u.opaque_sockaddr.sa_family == AF_INET6 ) \
         (_sock_g_p_)->sin_port = ((struct sockaddr_in6 *)(_sock_g_p_))->sin6_port; \
    else \
         (_sock_g_p_)->sin_port = ((struct sockaddr_in *)(_sock_g_p_))->sin_port;

char hostBfr1[ NI_MAXHOST ]; /* character string buffer for printable host ip name */
char hostBfr2[ NI_MAXHOST ]; /* character string buffer for printable host ip name */

/*  macros to assist in printing an IP address stored in a struct sockaddr */
/*  the ADDRESS_HALF_WORDS macro generates a list of 4 halfwords
**  from the 4 bytes of a socket s-addr field, in the correct order
**  for printing.   parm is pointer to s-addr field.
*/
#ifdef LEFT_BIG_ENDIAN
#define ADDRESS_HALF_WORDS(_s_addr_p_) \
      (unsigned short)(*((unsigned char *)(_s_addr_p_)))     \
     ,(unsigned short)(*(((unsigned char *)(_s_addr_p_))+1)) \
     ,(unsigned short)(*(((unsigned char *)(_s_addr_p_))+2)) \
     ,(unsigned short)(*(((unsigned char *)(_s_addr_p_))+3))
#else
#define ADDRESS_HALF_WORDS(_s_addr_p_) \
      (unsigned short)(*(((unsigned char *)(_s_addr_p_))+3)) \
     ,(unsigned short)(*(((unsigned char *)(_s_addr_p_))+2)) \
     ,(unsigned short)(*(((unsigned char *)(_s_addr_p_))+1)) \
     ,(unsigned short)(*((unsigned char *)(_s_addr_p_)))
#endif

#ifndef MSG_BLOK_SZ
#define MSG_BLOK_SZ 4096  /*  size of message block for send and receive */
#endif

#if defined(MSG_OUTPUT_TO_FILE) && ( ! defined(DYNAMIC_MSG_BUF) || (DYNAMIC_MSG_BUF != 4096) )
#error MSG_OUTPUT_TO_FILE requires preprocessor macro definition DYNAMIC_MSG_BUF=4096
#else /*  not MSG_OUTPUT_TO_FILE or DYNAMIC_MSG_BUF == 4096 */

/*   by default, use TCP,  but use UDP if defined
 *   note  -    IPSEND_SD and IPSEND are equivalent,
 *              but IPSEND_SD may generate more efficient machine code
 *              and should be used when the sd is already in a local variable
 */
#ifdef UDP
#define IPPROT_STRING "udp"
#define IPPROTCL IPPROTO_UDP
#define IPSOCKTYP SOCK_DGRAM
#define IPCONNECT(_sockfd_, _serv_addr_, _addrlen) (0)
#define IPSEND_SD(_sd_,_connindex_) sendto(_sd_ \
                      ,(outmsgblok+connection_array[_connindex_].con_blk_offset) \
                      ,(MSG_BLOK_SZ - connection_array[_connindex_].con_blk_offset) \
					   ,0 ,((const struct sockaddr *)&(connection_array[_connindex_].othrend_socket)), SOCKADDR_SIZE)
#define IPSEND(_connindex_) IPSEND_SD(connection_array[_connindex_].con_socket_desc,_connindex_)
#define IPRECV_SD(_sd_,_buf_,_len_,_flags_,_connindex_) ( socket_name_length = SOCKADDR_SIZE , recvfrom(_sd_,_buf_,_len_,_flags_ \
				,((struct sockaddr *)&(connection_array[_connindex_].othrend_socket)), &socket_name_length))
#define IPRECV(_buf_,_len_,_flags_,_connindex_) IPRECV_SD(connection_array[_connindex_].con_socket_desc,_buf_,_len_,_flags_,_connindex_)
#if (UDP < 5) /* use this value to control responder termination - see responderfn */
#undef UDP
#define UDP 5
#endif
#define NAMEINFO_FLAGS (NI_NUMERICHOST | NI_DGRAM) 
#else /* TCP */
#define IPPROT_STRING "tcp"
#define IPPROTCL IPPROTO_TCP
#define IPSOCKTYP SOCK_STREAM
#define IPCONNECT(_sockfd_, _serv_addr_, _addrlen) (connect(_sockfd_, _serv_addr_, _addrlen))
#define IPSEND_SD(_sd_,_connindex_) send(_sd_ \
                      ,(outmsgblok+connection_array[_connindex_].con_blk_offset) \
                      ,(MSG_BLOK_SZ - connection_array[_connindex_].con_blk_offset) \
                      ,0)
#define IPSEND(_connindex_) IPSEND_SD(connection_array[_connindex_].con_socket_desc,_connindex_)
#define IPRECV_SD(_sd_,_buf_,_len_,_flags_,_connindex_) recv(_sd_,_buf_,_len_,_flags_)
#define IPRECV(_buf_,_len_,_flags_,_connindex_) IPRECV_SD(connection_array[_connindex_].con_socket_desc,_buf_,_len_,_flags_,_connindex_)
#define NAMEINFO_FLAGS (NI_NUMERICHOST) 
#endif /* TCP */

#define CONN_SOCK_SIZE 64 /* size of cache of pointers to connection entries for lowest socket descriptors */
const int on = 1;
/*  extern int h_errno;  */
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
int getopt(int argc, char * const argv[],const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;
#include <signal.h>

#include <sys/select.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#undef LEFT_BIG_ENDIAN

#define _int64 long long
#define fopen64 fopen
#define tvunit tv_usec
#define TV_SCALE 1000        /*  amount by which to scale tv_units to bring into range 0-999 for printing */
#define TVFORMAT "06"        /*  format pattern when NOT scaled */
#define PERSEC 1000000       /*  number of tv_units per second  */
#define I64_FORMAT "ll"
#ifdef DEF_TMEZONE  /*  if linking multiple object decks together, only one may define tmezone */
/************
struct my_timezone
  {
    int tz_minuteswest;		** Minutes west of GMT.  **
    int tz_dsttime;		** Nonzero if DST is ever in effect.  **
  };
****************/
struct timezone tmezone = { 0 , 0 };
#endif /* DEF_TMEZONE  */
#define MONITOR_BEFORE (void) gettimeofday(&Tp_before ,&tmezone);
#define MONITOR_AFTER(_tm_) (void) gettimeofday(&Tp_after ,&tmezone);      \
                            _tm_.tv_sec += (Tp_after.tv_sec - Tp_before.tv_sec);     \
                            _tm_.tv_usec += (Tp_after.tv_usec - Tp_before.tv_usec);  \
                            if ( _tm_.tv_usec < 0 )                     \
                            {                                           \
                                 _tm_.tv_sec --;                        \
                                 _tm_.tv_usec += PERSEC;               \
                            }                                           \
                            else if ( _tm_.tv_usec >= PERSEC )         \
                            {                                           \
                                 _tm_.tv_sec ++;                        \
                                 _tm_.tv_usec -= PERSEC;               \
                            }
#define read_real_time(_aaa_, _bbb_) gettimeofday(_aaa_,0)
#define timebasestruct_t struct timeval
#define timestruc_t timeval
#define tb_high tv_sec
#define tb_low tv_usec
#define time_base_to_time(_aaa_, _bbb_) (0)

#define offset_t off_t

#define LONGLONGFORMAT "ll"
#define _O_CREAT  O_CREAT
#define _O_APPEND O_APPEND
#define _O_RDWR   O_RDWR
#define _S_IWRITE S_IWRITE
#define _open open

#include <sys/resource.h>
/* define RUSAGE_NONE to be a number that is neither RUSAGE_SELF nor RUSAGE_CHILDREN */
#define RUSAGE_NONE (((RUSAGE_SELF > RUSAGE_CHILDREN) ? RUSAGE_SELF : RUSAGE_CHILDREN) + 1)
#include <sys/ipc.h>
#include <sys/shm.h>
#include <memory.h>
#ifdef POLLSET
/*   the value assigned to the POLLSET macro alters behaviour :
     define POLLSET=2   to specify dynamic unregistering/registering of POLLOUT events based on whether we have anything to send
     define POLLSET=6   to specify dynamic unregistering/registering of POLLIN based on numwritten and POLLOUT events based on whether we have anything to send
     define POLLSET=32  to specify use of non-blocking sockets, edge-triggered polling and try send/recv on all sockets always
     define POLLSET=34  for both of the above
*/
#define EVENT_PRIORITIZATION_LEGEND " pollset"
#define POLLSET_DYNAMIC_REG 2
#define POLLSET_DYNAMIC_POLLIN 4  /*   dynamic unregistering/registering of pollin as well - probably degrades performance ...  */
#define POLLSET_EDGE_TRIG   32
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
#define POLLSET_DYN_LEGEND "-dynreg"
#else
#define POLLSET_DYN_LEGEND ""
#endif /* POLLSET & POLLSET_DYNAMIC_REG */
#if (POLLSET & POLLSET_EDGE_TRIG)
#define POLLSET_TRIG_LEGEND "-edge"
#else
#define POLLSET_TRIG_LEGEND "-level"
#endif /* (POLLSET & POLLSET_EDGE_TRIG)  */
#include <sys/epoll.h>
#define POLLEV_STRUCT struct epoll_event /* struct describing returned event */
#define POLLEV_FD data.fd                /* sd of returned event */
#define POLLEV_TYP events                /* type of returned event */
int pollset_fd;                          /* fd of the pollset    */
struct epoll_event pollctl_ev;          /* events of interest when adding sd */
#define POLLIN EPOLLIN
#define POLLOUT EPOLLOUT
#define POLLEV_GET_OR_WAIT(_pw_fd_ ,  _pw_ev_ ,  _pw_mx_ ,  _pw_tm_) epoll_wait(_pw_fd_ ,  _pw_ev_ ,  _pw_mx_ ,  _pw_tm_)
#define POLLSE_CREATE(_ps_sz_) epoll_create(_ps_sz_)
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  specify EPOLLET for edge-triggered */
#define POLLSE_ADDF(_fd_,_event_flags_) ( pollctl_ev.events = (_event_flags_ | EPOLLET), \
                           pollctl_ev.data.fd = _fd_, \
                           epoll_ctl(pollset_fd,EPOLL_CTL_ADD,_fd_,&pollctl_ev) )
#else /* !  (POLLSET & POLLSET_EDGE_TRIG)  */
#define POLLSE_ADDF(_fd_,_event_flags_) ( pollctl_ev.events = _event_flags_, \
                           pollctl_ev.data.fd = _fd_, \
                           epoll_ctl(pollset_fd,EPOLL_CTL_ADD,_fd_,&pollctl_ev) )
#endif /* !  (POLLSET & POLLSET_EDGE_TRIG)  */
#if (POLLSET & POLLSET_DYNAMIC_POLLIN)
/*  on linux, epoll_ctl MOD fails if not already in pollset, so try adding it first and only mod if add failed */
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  specify EPOLLET for edge-triggered */
#define POLLSE_MODF(_fd_,_event_flags_) ( pollctl_ev.events = (_event_flags_ | EPOLLET), \
                           pollctl_ev.data.fd = _fd_, \
                           ( ( epoll_ctl(pollset_fd,EPOLL_CTL_ADD,_fd_,&pollctl_ev) == 0 ) ? 0 : \
                           epoll_ctl(pollset_fd,EPOLL_CTL_MOD,_fd_,&pollctl_ev) ) )
#else /* !  (POLLSET & POLLSET_EDGE_TRIG)  */
#define POLLSE_MODF(_fd_,_event_flags_) ( pollctl_ev.events = _event_flags_, \
                           pollctl_ev.data.fd = _fd_, \
                           ( ( epoll_ctl(pollset_fd,EPOLL_CTL_ADD,_fd_,&pollctl_ev) == 0 ) ? 0 : \
                           epoll_ctl(pollset_fd,EPOLL_CTL_MOD,_fd_,&pollctl_ev) ) )
#endif /* !  (POLLSET & POLLSET_EDGE_TRIG)  */
#else /*  not POLLSET_DYNAMIC_POLLIN so we are always registered for POLLIN */
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  specify EPOLLET for edge-triggered */
#define POLLSE_MODF(_fd_,_event_flags_) ( pollctl_ev.events = (_event_flags_ | EPOLLET), \
                           pollctl_ev.data.fd = _fd_, \
                           epoll_ctl(pollset_fd,EPOLL_CTL_MOD,_fd_,&pollctl_ev) )
#else /* !  (POLLSET & POLLSET_EDGE_TRIG)  */
#define POLLSE_MODF(_fd_,_event_flags_) ( pollctl_ev.events = _event_flags_, \
                           pollctl_ev.data.fd = _fd_, \
                           epoll_ctl(pollset_fd,EPOLL_CTL_MOD,_fd_,&pollctl_ev) )
#endif /* !  (POLLSET & POLLSET_EDGE_TRIG)  */
#endif /*  not POLLSET_DYNAMIC_POLLIN so we are always registered for POLLIN */
#define POLLSE_DEL(_fd_) ( pollctl_ev.events = POLLIN|POLLOUT, \
                           pollctl_ev.data.fd = _fd_, \
                           epoll_ctl(pollset_fd,EPOLL_CTL_DEL,_fd_,&pollctl_ev) )

#define POLLSE_ADD(_fd_) POLLSE_ADDF(_fd_,(POLLIN|POLLOUT))
#define POLLSE_MOD(_fd_) POLLSE_MODF(_fd_,(POLLIN|POLLOUT))
int pollset_maxevents;
#else /*  SELECT */
#define EVENT_PRIORITIZATION_LEGEND " select"
#define POLLSET_DYN_LEGEND ""
#define POLLSET_TRIG_LEGEND ""
#endif /* SELECT */

/*  random number generator.
 *  in theory we should perform the multiplication in 64-bit precision
 *  but any processor that returns the correct low-order 32 bts of int-int multiplication is ok
 *  and we assume this is true unless user specifies -DRAND_INT_TYPE on his compilation.
 *  On 64-bit machines specifying -DRAND_INT_TYPE="unsigned long long" may improve performance.
 *  Note however that we must use long long for the scaling operation which follows the random num generation
 *       since for this case we need the high-order part.
 *  the congruence relation is new_random = (prev_random * a + c) mod M
 *  where M is 2**31.    Let MAX_RAND denote the max value in range, = 2**31 - 1
 *  These pseudo-random sequences have poor statistical properties but are adequate for this purpose and fast
 *
 *  extension to permit "good" generation of more than 2**31 distinct values:
 *  if (v - u + 1) (i.e. requested number of random values in range of generator) exceeds 2**31,
 *  then we will mantain two random streams in parallel, each of true period 2**31,
 *  and return the concatenation of the two as a 64-bit random value,
 *  but for the second series we will reject the value 2**31-1 (the max value) and iterate again if we encounter it,
 *  thus ensuring that its actual period has no common factor with the other.
 */
#ifndef RAND_INT_TYPE
/*  #define RAND_INT_TYPE unsigned long long  */
#define RAND_INT_TYPE unsigned int
#define SPRINTF_FORMAT "%d"
#define SAND_INT_TYPE          int
#define RAND_HNT_TYPE unsigned short
#define SAND_HNT_TYPE          short
#define LENG_INT_TYPE 4            /*   length of an int register  */
#define LENG_HNT_TYPE 2            /*   length of half an int register  */
#define MAX_HALF_REG SHRT_MAX  /* largest number of type SAND_HNT_TYPE (half an integer register) */
#define MAX_RAND 0x7fffffffu          /*  mask for all generators  */
#else
#undef RAND_INT_TYPE   /*  don't trust user to give me valid type ... */
#define RAND_INT_TYPE unsigned long long
#define SPRINTF_FORMAT "%lld"
#define SAND_INT_TYPE          long long
#define RAND_HNT_TYPE unsigned int
#define SAND_HNT_TYPE          int
#define LENG_INT_TYPE 8            /*   length of an int register  */
#define LENG_HNT_TYPE 4            /*   length of half an int register  */
#define MAX_HALF_REG INT_MAX  /* largest number of type SAND_HNT_TYPE (half an integer register) */
#define MAX_RAND 0x7fffffffULL        /*  mask for all generators  */

/*  declare the variables for the second random stream (for handling more than 2**31 distinct values) */
 RAND_INT_TYPE ran_inputval_sala = 0x0068d001;   /*   multiplicand */
 RAND_INT_TYPE ran_inputval_salc = 0x10010e55;   /*   addend       */
 RAND_INT_TYPE ran_inputval_salu = 0x03030303;   /*   initial seed */
#endif

/*  input value  */
 RAND_INT_TYPE ran_inputval_vala = 0x00777779;   /*   multiplicand */
 RAND_INT_TYPE ran_inputval_valc = 0x70707077;   /*   addend       */
 RAND_INT_TYPE ran_inputval_valu = 0x01010101;   /*   initial seed */
#ifdef USE_RAN_LIB
#if (LENG_INT_TYPE == 8)
#define RAN_ANYVAL(_a_, _c_, _u_, _min_val_ , _number_of_vals_) ( (_min_val_ + (RAND_INT_TYPE)(((unsigned long long)(_a_ = random(), _c_ = random(), (_a_ == MAX_RAND) && _a_ = random(), ((_a_ <<32) + _c_)) * (unsigned long long)_number_of_vals_) >>31)) )
#define RAN_INPUTVAL(_min_val_ , _number_of_vals_) RAN_ANYVAL(ran_inputval_sala, ran_inputval_salc, 0, _min_val_, _number_of_vals_)
#else
#define RAN_ANYVAL(_a_, _c_, _u_, _min_val_ , _number_of_vals_) ( (_min_val_ + (RAND_INT_TYPE)(((unsigned long long)random() * (unsigned long long)_number_of_vals_) >>31)) )
#define RAN_INPUTVAL(_min_val_ , _number_of_vals_) RAN_ANYVAL(0, 0, 0, _min_val_, _number_of_vals_)
#endif
#else /* use own congruence generator(s)   */
#define RAN_ANYVAL(_a_, _c_, _u_, _min_val_ , _number_of_vals_) ( ( _u_ = \
   ((unsigned int)(((RAND_INT_TYPE)_u_ * (RAND_INT_TYPE)_a_) + (RAND_INT_TYPE)_c_) & MAX_RAND) ) , \
   (_min_val_ + (RAND_INT_TYPE)(((unsigned long long)_u_ * (unsigned long long)_number_of_vals_) >>31)) )
#define RAN_INPUTVAL(_min_val_ , _number_of_vals_) RAN_ANYVAL(ran_inputval_vala, ran_inputval_valc, ran_inputval_valu, _min_val_, _number_of_vals_)
#endif


struct connection_info
{
  RAND_INT_TYPE con_ranvalu;      /*  random value for use with choosing when to send */
  struct sockaddr_general othrend_socket; /*   socket struct for other end of connection */
#ifdef MSG_OUTPUT_TO_FILE
  char *line_spanned_msgmall;         /* pointer to mallocd area containing inmsgblok described next */
  char *line_spanned_msgblok;         /* pointer to inmsgblok in which last line spans end of buffer */
  unsigned int line_spanned_offset;   /* offset in inmsgblok of next char to be output to file */
  int line_spanned_send_recv_len;     /*  length of received data in inmsgblok */
#endif  /* MSG_OUTPUT_TO_FILE */
  unsigned long con_numsent;      /*  num send calls  */
  unsigned long con_numrecv;      /*  num recv calls  */
  unsigned long con_nummsgw;      /*  num full messages written */
  unsigned long con_nummsgr;      /*  num full messages recvd   */
  unsigned long con_xsend_hwm;    /*  excess of (msgw - msgr) high-water-mark */
  int       con_socket_desc;      /*  socket descriptor   */
  unsigned int con_blk_offset;    /*  offset of next byte to send in out block   */
  unsigned int msgbytesrecv;  /* count of bytes recvd since previous eom, i.e. of bytes in a full message */
#ifdef POLLSET
  /*   the following bit field indicates two kinds of state :
  **     1.   poll has indicated a send or recv opportunity whch we shall defer taking
  **     2.   events of interest registered with pollset
  */
  int           pollset_flags;
#define POLLSET_RECV_OPPY  0x00000001      /*  poll has indicated a recv opportunity */
#define POLLSET_SEND_OPPY  0x00000002      /*  poll has indicated a send opportunity */
#define POLLSET_EV_POLLIN  0x00000010      /*  registered interest in POLLIN         */
#define POLLSET_EV_POLLOUT 0x00000020      /*  registered interest in POLLOUT        */
#endif /* POLLSET */
};
#define CONN_INFO_SIZE (((sizeof(struct connection_info) +7)>>3)<<3) /* roundup to multiple of 8 */

/*  user's input parms */
long iamenquirer, enquirerproccount, responderproccount, max_sendcount;
unsigned long lowestport;
char *enquirername;
char *respondername;
/*     for addrinfo - IPV6 enablement */
char enquirer_responder_separator;  /*  either : or ,  -  , requests use of IPV6 */
unsigned int  must_use_ipv6 = 0; /*  user insists on using IPV6    */
#define UNKNOWN_SCOPE_ID ~0
char local_net_intrfc[64];                           /*  local network interface to be used for IPV6  */
unsigned int  local_net_scope_id = UNKNOWN_SCOPE_ID; /*  scope zone index for local network interface */
struct addrinfo *enquire_addrP;
struct addrinfo *respond_addrP;
unsigned int maxenquirercount, maxrespondercount;

char jobname[124];

int tcp_rfc1323;
int tcp_nodelay;
int msg_waitall;
int recv_flags;
int sosndbufsz;            /*  size of network core buffers used for messages sent */
int sorcvbufsz;            /*  size of network core buffers used for messages received  */

int tabular;
int verbose;
int mimimal_out;   /*  if set, nothing except severe errors reported */

/*   fields associated with one particular process */
pid_t mypid;                  /*  process id */
long connectioncount;            /*  number of connections  */
long sockclosecount;               /*  number of connections that were open and have been closed */
int max_socket_desc;          /*  max socket descriptor number - used for select only */
int listensd;               /*  listener socket for responder  */
unsigned long sendcount;        /*  number of send calls so far  */
unsigned long recvcount;        /*  number of recv calls so far */
unsigned long msgoutcount;        /*  number of full messages written */
unsigned long msgincount;        /*  number of full messages recvd */
unsigned long excesssendcount;      /*  excess of (msgw - msgr) - currently */
#ifdef DYNAMIC_MSG_BUF        /*  malloc'd message buffers */
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers */
char *inmsgmall = 0;              /*  inbound message malloc'd area              */
char *outmsgmall = 0;             /*  outbound message malloc'd area             */
#endif
char *inmsgblok = 0;              /*  inbound message buffer  (one per process)  */
char *outmsgblok = 0;             /*  outbound message buffer   (one per process)  */
#else                         /*  if we don't want malloc'd message buffers */
char inmsgblok[MSG_BLOK_SZ];  /*  inbound message buffer  (one per process)  */
char outmsgblok[MSG_BLOK_SZ]; /*  outbound message buffer   (one per process)  */
#endif                        /*  static or malloc'd message buffers */
int proc_state;               /*  flags indicating conditions relating to process */
#define PROC_EAGAIN  0x00001000 /*  some request got EAGAIN - reset after each GetQueuedCompletionStatus in main loop */
#define PROC_ENOBUFS 0x00002000 /*  some request got ENOBUFS - reset after each GetQueuedCompletionStatus in main loop */
#define PROC_TRY_AGAIN (PROC_EAGAIN|PROC_ENOBUFS) /*  try initiating a Read or Write again */

volatile int signalled = 0;       /* signal received               */
/*  dynamic controls over printing  */
#define PRINT_ON        0x00000001    /* print lines */
#define PRINT_TIMESTAMP 0x00000008    /* print timestamps preceding lines */
volatile int printing_ctl = PRINT_ON; /* default is printing is enabled */
long shutting_down;    /*   indicates to try to shut down gracefully - accept input, dont send */
char *accept_con_pbuf_start;          /*  address of start of accept_connections print buffer */
#define ACCEPT_CON_PBUF_LEN 32768     /*  length of accept_connections print buffer */
char *accept_con_pbuf_cur;            /*  address of current write pointer in accept_connections print buffer */
struct itimerval time_limit = { { 0 , 0 }, { 0 , 0 } };     /*  limits time for job          */
struct itimerval interval_duration = { { 0 , 0 }, { 0 , 0 } };     /*  interval duration   */
struct itimerval sleeptime = { { 0 , 0 }, { 0 , 10000 } }; /*  10000 microsec = 10 millisec */
#define SIG_DUMP_OPS SIGURG

long expected_sendcount;     /*   if user specifies job duration by time_limit instead of max_sendcount,
                           **   this is our estimate of how many messages we expect, for our heuristics
                           */

char reportdir[256] = { '\0' };
FILE *reportFILE;
char netrush_string[8] = { 'n' , 'e' , 't' , 'r' , 'u', 's' , 'h' , '\0' };
char *command_nameP = 0;          /*  ->  name of command which invoked this program  */
#ifdef MSG_OUTPUT_TO_FILE
char msg_output_path[256] = { '\0' };  /*  write received msgs here if specified */
char my_msg_output_type = '\0';        /*  o for binary , O for linefeed-delimited  */
#define MY_MSG_OUTPUT_BINARY 'o'
#define MY_MSG_OUTPUT_LINEFD 'O'
#endif  /* MSG_OUTPUT_TO_FILE */
 
/*  printline prints to stdout/stderr and to reportFILE if defined
**  to facilitate printing socket IP addresses, we provide one extension :
**  a sequence of one or more pairs of characters "%K" at the front of the format string
**  are interpreted as indicating the same number of pairs of arguments sockaddr_ptr,string_ptr
**  at the front of the remaining argument list,  and this function formats the IP address
**  of each sockaddr into its corresponding string.
**  NOTE:   that (assuming the IP address is to be printed) the string_ptr must also be
**          included in the regular part of the format string and arg list.
*/
void printline(FILE *Stream, char *va_alist, ...)
{
#if defined(_LINUX_64) || defined(HAVE_STDARG)  /*  for gcc on ia64 we must use the stdarg macros as the parm list is not in "natural" format */
#include <stdarg.h>
  va_list vargp;
#else
  char * vargp;
#endif
  char datefield[18];
  const char *msg;
  struct timeval curtime = { 0 , 0 };
  struct timezone curzone = { 0 , 0 };
  struct sockaddr_general *sockaddr_ptr;
  char *string_ptr;
  int num_ips;    /*  number of IP address/string-pointer pairs in the arg list */
  int ix;

  if (printing_ctl & PRINT_ON)
  {
#if defined(_LINUX_64) || defined(HAVE_STDARG)  /*  for gcc on ia64 we must use the stdarg macros as the parm list is not in "natural" format */
    va_start(vargp ,va_alist); /*  initialise vargp -> beginning of arg list */
    msg = va_alist;      /*  address second parm - format string   */
#else
    vargp = (char *) &va_alist;    /*  initialise vargp -> beginning of arg list */
    msg = *((char **)vargp);                           /*  address first parm    */
    vargp += sizeof(char *);                           /*  advance parm pointer  */
#endif

    /*  convert IP addresses first  */
    num_ips = 0;
    while ((*msg == '%') && (*(msg+1) == 'K')) {
#if defined(_LINUX_64) || defined(HAVE_STDARG)
      sockaddr_ptr = (struct sockaddr_general *)va_arg(vargp, char *);
      string_ptr = va_arg(vargp, char *);
#else
      sockaddr_ptr = *((struct sockaddr **)vargp);
      vargp += sizeof(char *);
      string_ptr = *((char **)vargp);
      vargp += sizeof(char *);
#endif
      if (getnameinfo((struct sockaddr *)sockaddr_ptr, sizeof(struct sockaddr_general), string_ptr, NI_MAXHOST, 0, 0, NAMEINFO_FLAGS) != 0) {
        fprintf(stderr,"%s printline:  getnameinfo of socket_addr at 0x%p family %hu port %hu reports errno %d\n",command_nameP
                ,sockaddr_ptr , sockaddr_ptr->sockaddr_u.opaque_sockaddr.sa_family ,sockaddr_ptr->sin_port, errno);
        *string_ptr = '\0';
      }
      msg += 2;
      num_ips++;
    }

    if (printing_ctl & PRINT_TIMESTAMP)
    {
      gettimeofday(&curtime,&curzone);
      memcpy(datefield,ctime((time_t *)&(curtime.tv_sec))+4,16);
      datefield[16] = ' ';
      datefield[17] = '\0';
    }

    if (Stream && (!(mimimal_out)))
    {
      if (printing_ctl & PRINT_TIMESTAMP)
        fprintf(Stream,"%s ",datefield);
      vfprintf(Stream, msg, vargp);
      fflush(Stream);
    }
    if (reportFILE)
    {
      ix = 2 * num_ips;  /*  2 parms per IP */
      if (printing_ctl & PRINT_TIMESTAMP)
        fprintf(reportFILE,"%s ",datefield);
#if defined(_LINUX_64) || defined(HAVE_STDARG)  /*  for gcc on ia64 we must use the stdarg macros as the parm list is not in "natural" format */
    va_end(vargp);
    va_start(vargp ,va_alist); /*  initialise vargp -> beginning of arg list */
#else
    vargp = (char *) &va_alist;    /*  initialise vargp -> beginning of arg list */
    vargp += sizeof(char *);                           /*  advance parm pointer  */
#endif
      while (ix-- > 0) {            /*    step past the IP-address/string_ptr pairs */
#if defined(_LINUX_64) || defined(HAVE_STDARG)
        va_arg(vargp, char *);
#else
        vargp += sizeof(char *);
#endif
      }
        
      vfprintf(reportFILE, msg, vargp);
      fflush(reportFILE);
    }
#if defined(_LINUX_64) || defined(HAVE_STDARG)  /*  for gcc on ia64 we must use the stdarg macros as the parm list is not in "natural" format */
    va_end(vargp);
#endif
  }
}

struct supvr_workr_common                  /*  area specific to one worker and shared with supervisor */
{
  unsigned long ch_num_sent;         /*  send calls so far */
  unsigned long ch_num_recv;         /*  recv calls so far */
  unsigned long ch_num_msgw;         /*  full msgs written so far */
  unsigned long ch_num_msgr;         /*  full msgs received so far */
  unsigned long ch_xsend_hwm;        /*  excess of (msgw - msgr) high-water-mark */
  unsigned long ch_num_selpol;       /*  number of calls to select or poll_wait */
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
  unsigned long ch_num_dynreg;       /*  number of calls to dynamically register event */
  unsigned long ch_num_dynung;       /*  number of calls to dynamically unregister event */
#endif /* POLLSET & POLLSET_DYNAMIC_REG */
#if (POLLSET & POLLSET_EDGE_TRIG)    /*  count EAGAIN and EWOULDBLOCK for edge-triggered */
  unsigned long ch_num_wdblck;       /*  number of cases of EAGAIN and EWOULDBLOCK for edge-triggered */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG) count EAGAIN and EWOULDBLOCK for edge-triggered */
  double        ch_avg_send_len;     /*  average length of bytes sent per send */
  double        ch_avg_recv_len;     /*  average length of bytes rcvd per recv */
  pid_t         ch_pid;              /*  process id */
  int           ch_flags;            /*  flags     */
#define CH_CHILD_FINISHED 0x00001000 /*  worker has finished all useful work but may be lingering for some reason */
  unsigned int othrend_msg_blok_sz;  /*  length of full application msg block sent by other end */
};
struct supvr_workr_common *supvr_workrP; /*  pointer to my common area */
volatile pid_t *pidarray = 0;        /*  array of pids to be terminated  */
unsigned long pidindex;              /*  index of current worker   */

struct rusage RUsage;         /*   total rusage for life of process   */
struct timeval curtime = { 0 , 0 };
struct timezone curzone = { 0 , 0 };

sigset_t emptymask, blockusermask, blockalarmask, prevmask, tempmask;
typedef struct sigaction SIGACTYPE;
SIGACTYPE chsigaction, olsigINTaction, olsigTRMaction, olsigHUPaction, olsigDGRaction
              ,olsigUS1action, olsigUS2action, olsigALRMaction, olsigWCHaction, olsigPWRaction, olsigXCPaction, olsigPREaction, olsigPIPaction;

timebasestruct_t start_time, finish_time;
timebasestruct_t accum_time_base; /* accumulated delta time_base for send/receive */
long long *accum_timep;          /* ->   accumulated delta time for send/receive */

char person[2][7] =  { { 'c', 'h', 'i', 'l', 'd', ' ', '\0' },
                       { 'p', 'a', 'r', 'e', 'n', 't', '\0' }, };
char specie[2][7] =  { { 's', 'e', 'r', 'v', 'e', 'r', '\0' },
                       { 'c', 'l', 'i', 'e', 'n', 't', '\0' }, };
/*  we hope the following files exist and are fsacl'ble on all systems - for use as ftok arguments  */
char reliable_file[2][14] =  { { '/', 'e', 't', 'c', '/', 's', 'e', 'r', 'v', 'i', 'c', 'e', 's', '\0' },
                               { '/', 'e', 't', 'c', '/', 'g', 'r', 'o', 'u', 'p', '\0', '\0', '\0', '\0' }, };

void handlSIGS(int sigsent, int code)
{
  /*  we are invoked whenever certain signals are issued and record the signal delivery in the common field  */
  /*  special interpretation of certain signals */
  if (sigsent == SIGUSR1)
  {
    printing_ctl |= PRINT_ON;
    verbose = 1;
  }
  else if (sigsent == SIGUSR2)
  {
    printing_ctl |= PRINT_ON;
    verbose = 2;
  }
  else if (sigsent == SIGWINCH)
  {
    if (printing_ctl & PRINT_ON)
      printing_ctl &= ~( PRINT_ON|PRINT_TIMESTAMP );
    else printing_ctl |= PRINT_ON;
    verbose = 0;
  }
#ifdef SIGPWR
  else if (sigsent == SIGPWR)
  {
    if (printing_ctl & PRINT_TIMESTAMP)
      printing_ctl &= ~PRINT_TIMESTAMP;
    else printing_ctl |= PRINT_TIMESTAMP;
  }
#endif  /*  def SIGPWR  */
  else signalled = sigsent;

  return;
}

int print_nameinfo(struct sockaddr_general *sockgen_p , socklen_t sockgen_l) {
  int rc;
  struct sockaddr_in   *pSadrIn;
  struct sockaddr_in6  *pSadrIn6;
  char  servBfr[ NI_MAXSERV ];
      if ((rc = getnameinfo( (struct sockaddr *)sockgen_p ,sockgen_l
                ,hostBfr1 ,sizeof( hostBfr1 ) ,servBfr ,sizeof(servBfr) ,(NAMEINFO_FLAGS|NI_NUMERICSERV))) != 0) {
            printline(stderr,"%s print_nameinfo:  getnameinfo of socket_addr at 0x%p family %hu port %hu reports errno %d\n",command_nameP
                      ,sockgen_p , sockgen_p->sockaddr_u.opaque_sockaddr.sa_family ,sockgen_p->sin_port, errno);
                   hostBfr1[0] = '\0'; servBfr[0] = '\0';
      }

      switch ( sockgen_p->sockaddr_u.opaque_sockaddr.sa_family )
      {
        case PF_INET:   /* IPv4 address record. */
        {
           pSadrIn = (struct sockaddr_in *) sockgen_p;
           printline( stderr
                   ,"   ai_addr      = sin_family: %d (AF_INET = %d, AF_INET6 = %d)\n"
                    "                  sin_addr:   %s  sin_port:     %s\n"
                   ,pSadrIn->sin_family
                   ,AF_INET
                   ,AF_INET6
                   ,hostBfr1,servBfr
                   );
           break;
        }  /* End CASE of IPv4 record. */
        case PF_INET6:   /* IPv6 address record. */
        {
           pSadrIn6 = (struct sockaddr_in6 *)sockgen_p;
           printline( stderr
                   ,"   ai_addr      = sin6_family:   %d (AF_INET = %d, AF_INET6 = %d)\n"
                    "                  sin6_addr:     %s   sin6_port:     %s\n"
                    "                  sin6_flowinfo: %d\n"
                    "                  sin6_scope_id: %d\n"
                   ,pSadrIn6->sin6_family
                   ,AF_INET
                   ,AF_INET6
                   ,hostBfr1,servBfr
                   ,pSadrIn6->sin6_flowinfo
                   ,pSadrIn6->sin6_scope_id );
           break;
        }  /* End CASE of IPv6 record. */
        default:   /* Can never get here, but just for completeness. */
        {
           printline( stderr
                   ,"netrush: (line %d): ERROR - Unknown protocol family (%d).\n"
                   ,__LINE__
                      , sockgen_p->sockaddr_u.opaque_sockaddr.sa_family );
           rc = 29;
           goto finish_printinf;
        }  /* End DEFAULT case (unknown protocol family). */
      }  /* End SWITCH on protocol family. */
 finish_printinf:
  return rc;
}

int init_outmsg_buff(          /*  initialize the outmsg buffer  */
                      pid_t mypid
                     ,char *outmsgblok
                     ,const char *message_banner_p
                    )
{
  int rc = 1;
  int kx;

  *(outmsgblok) = message_som;      /*  start-of-msg indicator   */
  strcpy(outmsgblok+1, message_banner_p);
  kx = sprintf(outmsgblok+MESSAGE_SOMBAN_SIZE, "%d ", mypid);   /*  length of characters including blank */
  if (kx < (MESSAGE_HDR_SIZE-MESSAGE_SOMBAN_SIZE))
    memset(outmsgblok+MESSAGE_SOMBAN_SIZE+kx, ' ', (MESSAGE_HDR_SIZE-MESSAGE_SOMBAN_SIZE-kx)); /*  blank remaining header characters */
  else if (kx > (MESSAGE_HDR_SIZE-MESSAGE_SOMBAN_SIZE))
  {
    printline(stderr,"pid %d too long in outmsg %s\n",mypid,outmsgblok+1);
    goto termi;
  }
  memset(outmsgblok+MESSAGE_HDR_SIZE, 0, ((MSG_BLOK_SZ-MESSAGE_HDR_SIZE)));  /*  ensure all non-message bytes are zero */
  *(outmsgblok+((MSG_BLOK_SZ-1))) = message_eom;      /*  end-of-msg indicator   */
  rc = 0;
 termi:
  return rc;
}


long find_connection(        /*  return the connection index for connection using socket sd */
                                     /*  return <0 for invalid socket or connection not found */
                            int sd   /*  socket  */
              ,short *conn_by_socket /*  cache of responder indexes  */
              ,long numconnections   /*  number of connections    */
              ,struct connection_info *connection_array
                    )
{
  long responderindex;

  if (sd < 0)                    /*  invalid socket number      */
    return -1;
  else if (sd < CONN_SOCK_SIZE)  /*  connection index is cached */
    return (long)conn_by_socket[sd];
  else                           /*  search connection list     */
  {
    for (responderindex = 0; responderindex < numconnections; responderindex++)
    {
      if (connection_array[responderindex].con_socket_desc == sd) /* this connection */
        return responderindex;
    }
  }
  return -1;                     /*  not found                  */
}

long close_connection(               /*  close the connection that is using socket sd */
                                     /*  return negative number in range -255 , -1 for invalid socket or connection not found */
                                     /*  return -4097 if there exist in-flight reads or writes */
#define PENDING_CLOSE_IN_FLIGHT ~PENDING_CLOSE_CON
                                     /*  otherwise return new max socket descr */
                            int sd   /*  socket  */
              ,short *conn_by_socket /*  cache of responder indexes  */
              ,long numconnections   /*  number of connections    */
              ,struct connection_info *connection_array
              ,int *responderproc_array /*  array of indexes of all responder processes permuted randomly
                                     **  (only for enquirer, null for responder) */
                    )
{
  long connindex, ix, jx;
  int rc;
  connindex = find_connection(sd, conn_by_socket, numconnections, connection_array); /* locate connection */
  if (connindex >= 0)
  {
    shutdown(sd,SHUT_RDWR);
    if (sd < CONN_SOCK_SIZE)  /*  connection index is cached */
      conn_by_socket[sd] = -1;
    connection_array[connindex].con_socket_desc = -1;
    connectioncount--;
    sockclosecount++;
#ifdef POLLSET
    if (!(rc = POLLSE_DEL(sd)))    /*   remove from our pollset  */
    {
      connection_array[connindex].pollset_flags &= ~(POLLSET_EV_POLLIN|POLLSET_EV_POLLOUT); /* now no registered events */
              /*    if (verbose > 1)  */
                      printline(stderr,"close_connection: %s %d: deleted socket %d connindex %ld from set\n",specie[iamenquirer],mypid,sd,connindex);
#else  /* not POLLSET */
                      printline(stderr,"close_connection: %s %d: removed socket %d connindex %ld from connection_array\n",specie[iamenquirer],mypid,sd,connindex);
#endif /* POLLSET */
    if (responderproc_array)  /*  if enquirer  */
    {
      /*  move this responder index to the end of the responderproc_array */
      for (ix = 0; ix < numconnections; ix++)
      {
        if (responderproc_array[ix] == connindex)
        {
          for (jx=ix; jx < (numconnections-1); jx++)
            responderproc_array[jx] = responderproc_array[jx+1];
          responderproc_array[jx] = connindex;
          break;
        }
      }
      if (ix == numconnections)
      {
        printline(stderr,"close_connection: %s %d: can't find conn index %ld in connection_array\n",specie[iamenquirer],mypid,connindex);
        connindex = -129;
        goto fin_close;
      }
    }
    /*   find max socket descr and store in connindex */
    connindex = 0;
    for (ix = 0; ix < numconnections; ix++)
    {
      if (connection_array[ix].con_socket_desc > connindex)
        connindex = connection_array[ix].con_socket_desc;
    }
    max_socket_desc =((responderproc_array || (connindex > listensd)) ? connindex : listensd);
#ifdef POLLSET
    }
    else
    {
      rc = errno;
      printline(stderr,"close_connection: %s %d: connindex %d pollctl del sd %d errno %d %s\n",specie[iamenquirer],mypid,connindex,sd,rc,strerror(rc));
      connindex = -(rc+8192);
      goto fin_close;
    }
#endif /* POLLSET */
  }
 fin_close:
  return connindex;
}

int get_finish_time(int who, timebasestruct_t *gft_start_time, timebasestruct_t *gft_finish_time, timebasestruct_t *gft_accum_time_base)
{
  int rc;
  read_real_time(gft_finish_time, TIMEBASE_SZ);

    if (who != RUSAGE_NONE)
        getrusage(who, &RUsage);

    curtime.tv_sec = 0;
    curtime.tv_usec = 0;
    curzone.tz_minuteswest = 0;
    curzone.tz_dsttime = 0;
    gettimeofday(&curtime,&curzone);
    accum_timep = (long long *) &(gft_accum_time_base->tb_high);
      gft_accum_time_base->tb_high = gft_finish_time->tb_high - gft_start_time->tb_high;
      gft_accum_time_base->tb_low =  gft_finish_time->tb_low  - gft_start_time->tb_low;
      while (gft_accum_time_base->tb_low < 0)
      {
        gft_accum_time_base->tb_high -= 1;
        gft_accum_time_base->tb_low  += PERSEC;
      }
      while (gft_accum_time_base->tb_low > PERSEC)
      {
        gft_accum_time_base->tb_high += 1;
        gft_accum_time_base->tb_low  -= PERSEC;
      }

  return 0;
}

int handle_send_error
                         /*    handle error on send, return indicator of whether : 0 <=> retry  or  non-0 <=> unrecoverable */
              (int sd
              ,long connindex        /*  connection index         */
              ,short *conn_by_socket /*  cache of responder indexes  */
              ,long numconnections   /*  number of connections    */
              ,struct connection_info *connection_array
              ,int *responderproc_array /*  array of indexes of all responder processes permuted randomly
                                     **  (only for enquirer, null for responder) */
              ,int err_num           /*  errno */
              )
{
  int rc;
  int temp_rc;

  if (signalled == SIGTERM)
    shutting_down = 1;   /*  commence graceful termination */
  rc = err_num;
  /*   for ECONNRESET on send, we simply close this connection  */
  if (   (rc == ENOTCONN) || (rc == ECONNRESET) || (rc == EPIPE) )
  {
    if ((temp_rc = close_connection(sd, conn_by_socket, numconnections, connection_array, responderproc_array)) >= 0)
    {
      printline(((verbose > 2) ? stderr : 0),"handle_send_error: %s %d: closed conn index %ld socket desc %d errno %d\n",specie[iamenquirer],mypid,connindex,sd,err_num);
        /*******************************************************************************************************
        **  logic for termination of enquirer and responder here
        *******************************************************************************************************/
#ifndef AD_NAUSEAM
      if (sockclosecount >= numconnections) /*  if closed all connections */
        shutting_down = 1;             /*  commence graceful termination */
#endif
      goto handled;
    }
    else
    {
        printline(stderr,"handle_send_error: %s %d: can't find connindex for socket %d\n",specie[iamenquirer],mypid,sd);
        shutting_down = 1;   /*  commence graceful termination */
      goto handled;
    }
  }
  if (rc == EINTR || rc == EAGAIN)
  {
    if (rc == EAGAIN)
      proc_state |= PROC_EAGAIN;   /*  remember we have received an EAGAIN */
    rc = 0; /* retry */
    goto handled;
  }
  printline(stderr,"handle_send_error: %s %d: responder index %d socket desc %d send errno %d %s\n",specie[iamenquirer],mypid,connindex,sd,rc,strerror(rc));

  /*  for ENOBUFS, (no buffer space), don't retry but don't shut down - caller can go wait on another GetQueuedCompletionStatus */
  if (rc == ENOBUFS)
    proc_state |= PROC_ENOBUFS;   /*  remember we have received an ENOBUFS */
  else
    shutting_down = 1;   /*  commence graceful termination */
 handled:
  return rc;
}

int handle_recv_error
                         /*    handle error on recv, return indicator of whether : 0 <=> retry  or  non-0 <=> unrecoverable */
              (int sd                /*  socket descriptor */
              ,long connindex        /*  connection index         */
              ,short *conn_by_socket /*  cache of responder indexes  */
              ,long numconnections   /*  number of connections    */
              ,struct connection_info *connection_array
              ,int *responderproc_array /*  array of indexes of all responder processes permuted randomly
                                     **  (only for enquirer, null for responder) */
              ,int err_num           /*  errno  (not set if send_recv_len >= 0)  */
              ,int send_recv_len     /*  length of msg received  - caller must set to <0 if err_num is set */
              )
{
  int rc;
  int temp_rc;

  if (signalled == SIGTERM)
    shutting_down = 1;   /*  commence graceful termination */
  rc = err_num;
  /*   for ECONNRESET on recv, we simply close this connection  */
  if (   (rc == ENOTCONN) || (rc == ECONNRESET)  || (rc == EPIPE)|| (send_recv_len == 0)
#if 0
                                    /*   the following causes some kind of race resulting in processes exiting early */
#ifndef AD_NAUSEAM
                                    || ((send_recv_len == 1) && (inmsgblok[0] == stream_eof[0])) /* or the one-byte stream eof indicator */
#endif
#endif
     )
  {
    rc = 1;   /*  unrecoverable  */
    if ((temp_rc = close_connection(sd, conn_by_socket, numconnections, connection_array, responderproc_array)) >= 0)
    {
      printline(((verbose > 2) ? stderr : 0),"handle_recv_error: %s %d: closed conn index %ld socket desc %d errno %d send_recv_len %d\n",specie[iamenquirer],mypid,connindex,sd,err_num,send_recv_len);
        /*******************************************************************************************************
        **  logic for termination of enquirer and responder here
        *******************************************************************************************************/
#ifndef AD_NAUSEAM
                                    if (sockclosecount >= numconnections)   /*  if closed all connections */
                                      shutting_down = 1;             /*  commence graceful termination */
#endif
      goto handled;
    }
    else
    {
        printline(stderr,"handle_recv_error: %s %d: can't find connindex for socket %d\n",specie[iamenquirer],mypid,sd);
        shutting_down = 1;   /*  commence graceful termination */
      goto handled;
    }
  }
  if (rc == EINTR || rc == EAGAIN)
  {
    if (rc == EAGAIN)
      proc_state |= PROC_EAGAIN;   /*  remember we have received an EAGAIN */
    rc = 0; /* retry */
    goto handled;
  }
  printline(stderr,"handle_recv_error: %s %d: conn index %ld socket desc %d recv errno %d %s\n",specie[iamenquirer],mypid,connindex,sd,rc,strerror(rc));

  /*  for ENOBUFS, (no buffer space), don't retry but don't shut down - caller can go wait on another GetQueuedCompletionStatus */
  if (rc == ENOBUFS)
    proc_state |= PROC_ENOBUFS;   /*  remember we have received an ENOBUFS */
  else
   shutting_down = 1;   /*  commence graceful termination */
  handled:
  return rc;
}

int handle_send_completion           /*  a send has completed with rc != 0 ==>  successful */
                                     /*  maintain counters and connection info, and for iocp enquirer, start new send
                                     **  return 0 if no error or non-zero if error
                                     */
              (struct supvr_workr_common *chshmp /*  pointer to worker shared-mem area */
              ,int sd                /*  socket   */
              ,long connindex        /*  connection index  */
              ,short *conn_by_socket /*  cache of responder indexes  */
              ,long numconnections   /*  number of connections    */
              ,struct connection_info *connection_array
              ,int *responderproc_array /*  array of indexes of all responder processes permuted randomly
                                     **  (only for enquirer, null for responder) */
              ,int send_recv_len     /*  length of msg sent */
              )
{
 int rc = 0;
 int temp_rc;
  int send_recv_len_l;           /*  local length of msg sent */
  int errno_l;                   /*  local copy of errno  */

  send_recv_len_l = send_recv_len; /*  local length of msg sent */

 handle_another_send:
  if (send_recv_len_l > 0)
  {
    sendcount++;
    connection_array[connindex].con_numsent++;
    chshmp->ch_num_sent++;
    if (iamenquirer)
    {
      /*   if we have used up all of our budget for max_sendcount, then start to shut down */
      if ((max_sendcount > 0) && (sendcount >= max_sendcount))
        shutting_down = 1;   /*  commence graceful termination */
    }
    chshmp->ch_avg_send_len += ((double)send_recv_len_l - chshmp->ch_avg_send_len)/(double)(chshmp->ch_num_sent);
#ifdef DEBUG_NETRUSH
    printline(stderr,"handle_send_completion: %s %d: sent %d bytes to connection %d socket %d sentmsgs %d of %d content %s %s\n"
              ,specie[iamenquirer],mypid,send_recv_len_l,connindex,sd,connection_array[connindex].con_sendcount,sendcount
              ,((*(outmsgblok) == message_som) ? (outmsgblok+1) : "?")
              ,((*(outmsgblok) == message_som) ? (outmsgblok+MESSAGE_NUMWRITN_OFFSET) : "!")
             );
#endif
    connection_array[connindex].con_blk_offset += send_recv_len_l;
    if (connection_array[connindex].con_blk_offset >= MSG_BLOK_SZ)
    {
      msgoutcount++;          /*   count of full messages written */
      connection_array[connindex].con_nummsgw++;
      chshmp->ch_num_msgw++;
      connection_array[connindex].con_blk_offset = 0;
      /* excess of (msgw - msgr) high-water-mark */
      if (connection_array[connindex].con_nummsgw > connection_array[connindex].con_nummsgr)
        excesssendcount = (connection_array[connindex].con_nummsgw - connection_array[connindex].con_nummsgr);
      else excesssendcount = 0;
      if (excesssendcount > connection_array[connindex].con_xsend_hwm)
        connection_array[connindex].con_xsend_hwm = excesssendcount;
      if (excesssendcount > chshmp->ch_xsend_hwm)
        chshmp->ch_xsend_hwm = excesssendcount;
    }
  }
 return_send_completion:
  return rc;
}

int handle_recv_completion           /*  a recv has completed with rc != 0 ==>  successful */
                                     /*  maintain counters and connection info
                                     **  return 0 if no error or non-zero if error
                                     */
              (struct supvr_workr_common *chshmp /*  pointer to worker shared-mem area */
              ,int sd                /*  socket            */
              ,long connindex        /*  connection index  */
              ,short *conn_by_socket /*  cache of responder indexes  */
              ,long numconnections   /*  number of connections    */
              ,struct connection_info *connection_array
              ,int *responderproc_array /*  array of indexes of all responder processes permuted randomly
                                     **  (only for enquirer, null for responder) */
              ,int send_recv_len     /*    length of IP packet msg sent or received */
                           )
{
  int rc = 0;
  int temp_rc;
  int send_recv_len_l;           /*  local length of msg received */
  int errno_l;                   /*  local copy of errno  */
  int ix;
  int bx;                        /* local copy of count of bytes in message */
  char *charptr;

  send_recv_len_l = send_recv_len; /*  local length of msg received */

  /* handle completion  */
 handle_another_recv:
  recvcount++;
  connection_array[connindex].con_numrecv++;
  chshmp->ch_num_recv++;
  chshmp->ch_avg_recv_len += ((double)send_recv_len_l - chshmp->ch_avg_recv_len)/(double)(chshmp->ch_num_recv);
  /*  count end-of-message indicators */
  for (bx = 0,ix = 0,charptr = inmsgblok; ix<send_recv_len_l; ix++,charptr++) {
    bx++;
    if (*charptr == message_eom)
    {
      msgincount++;
      connection_array[connindex].con_nummsgr++;
      chshmp->ch_num_msgr++;
      chshmp->othrend_msg_blok_sz = connection_array[connindex].msgbytesrecv + bx;  /*  always overwrite whatever was there */
      connection_array[connindex].msgbytesrecv = bx = 0;
    }
  }
  connection_array[connindex].msgbytesrecv += bx;  /*  increment for next msg */

#ifdef DEBUG_NETRUSH
  printline(stderr,"handle_recv_completion %s %d: recv %d bytes from connection %d socket %d recvmsgs %d of %d content %s %s\n"
            ,specie[iamenquirer],mypid,send_recv_len_l,connindex,sd,connection_array[connindex].con_recvcount,recvcount
              ,((*(inmsgblok) == message_som) ? (inmsgblok+1) : "?")
              ,((*(inmsgblok) == message_som) ? (inmsgblok+MESSAGE_NUMWRITN_OFFSET) : "!")
           );
#endif

 return_recv_completion:
  return rc;
}


/*  enquirerfn - all processing performed by the enquirer - called as single supervisor process  */
int enquirerfn()
{
  int rc;
  int selectrc = 0;          /*  return code from select */
  int sd;                /*  arbitrary socket descriptor */
  struct connection_info *connection_array;  /*  array of all connections,
                                                 ordered by responder index, including socket descriptors  */
  int *responderproc_array; /*  array of indexes of all responder processes permuted randomly */
  int Imsupervisor;
  pid_t otherpid;
  char *socket_blocks_p;
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
  char *socket_blocks_all;    /*  mallocd socketblocks area     */
#endif                        /*  page-aligned message buffers */

  struct sockaddr_general source_socket;      /*       a source socket struct */
  struct sockaddr_general *target_socket_p;   /*  ->   a target socket struct */
  socklen_t socket_name_length;

  volatile long numforked;       /* number of processes forked by this supervisor */
  /*  we depend on value of numforked in this worker being same as it was in supervisor whern forking us
  **  (for assigning source port number, see reference to numforked below)
  **  hence volatile
  */
  long responderindex, ix, jx, kx;

  unsigned short current_port_num;
  long send_criterion; /*  random in range [ 0 , (total_numsent+(max_sendcount/6)) ] to decide on sending */
  /*   the following are parms for select */
  int Nfdsmsgs;
  int select_list_size;
  /*   int max_socket_desc;    now global */
  fd_set readlist;
  fd_set writelist;
  /***   fd_set ExceptList;   ***/
  struct timeval TimeOut = { 0 , 500000 }; /* set timeout to 0.5 sec */
  int send_recv_len;                       /*  length of IP packet msg sent or received */

  char *charptr;
  long *fdwordp;   /*  pointer to a select fdlist word, i.e. a sequence of bits representing fds
                   **  note that fd numbers ascend from low to high within a word of type long.
                   **        Remarkably, this is true even though the size of a long differs between 32-bit and 64-bit -
                   **        somehow the function "knows" what size my application is compiled to use (!)
                   **        so, e.g.    -       in 32-bit, fd 5 is located here      0x2ff1bd88:  00000020
                   **        so, e.g.    -  but  in 64-bit, fd 5 is located here      0x0fffffffffff8a70:  00000000 00000020
                   */
  short conn_by_socket[CONN_SOCK_SIZE]; /* cache of responder indexes for lowest socket descriptors
                                        ** must be signed since we use -1 to indicate none
                                        */
  char my_reportdir[256];
  long skipped_a_send;   /*  whether we skipped a send in order to give another connection a chance */
  struct supvr_workr_common *chshmp;         /*  pointer to worker shared-mem area */

#ifdef POLLSET
        POLLEV_STRUCT *pollset_events;    /*   ->  array of "completed" pollset events */
        long evx;                         /*   index into pollset_events array */
#if (POLLSET & POLLSET_EDGE_TRIG)
        long edge_triggered_event;        /*   flag to track occurrence of edge_triggered_events */
#else
        long send_recv_oppy_count;        /*   count of number of connections that have a deferred opportunity for send or recv */
        long pollin_oppys_count;          /*   count of number of pollin opportunities from most recent select - for deciding whether to terminate */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  */
#endif /* POLLSET */
        struct itimerval resume_time_limit; /*  limits time for job   -  save and restore remaining interval         */
/*     for addrinfo - IPV6 enablement */
       unsigned int enquirer_addrinfo_index;  /*  index into enquire_addrP array of the IP address to use  */
       unsigned int responder_addrinfo_index;  /*  index into respond_addrP array of the IP address to use  */

  shutting_down = 0;

  /*   fork worker processes */
  for (numforked = 0; numforked < enquirerproccount; numforked++)
  {
    if ( ! ( rc = Imsupervisor = otherpid = fork() ) ) /* fork worker */
    {
      /* zero retcd means I am worker */
      socket_blocks_p = 0;
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
     socket_blocks_all = 0;   /*  mallocd socketblocks area     */
#endif                        /*  page-aligned message buffers  */
      ran_inputval_valu = mypid = getpid(); /* my pid seeds random number generator */
    look_for_pid:
      for (ix = 0; (pidarray && (
                    jx = pidarray[ix])); ix++)
      {
        if (jx == mypid)  goto got_mypid;
      }
    
      if (time_limit.it_value.tv_sec > 0)
      {
        getitimer ( ITIMER_REAL, &resume_time_limit);
      }
      setitimer ( ITIMER_REAL, &sleeptime, 0);
      sigprocmask(SIG_BLOCK,&blockalarmask,&tempmask); /*  temporarily block signal ALRM... */
      while (!signalled)                               /*  check we have received the correct signal */
          sigsuspend(&tempmask);                       /*   wait for signal             */
      sigprocmask(SIG_SETMASK,&tempmask,0);            /*  normal mask allows ALRM */
      signalled = 0;                                   /*   reset indicator        */

      if (time_limit.it_value.tv_sec > 0)
      {
        setitimer ( ITIMER_REAL, &resume_time_limit, 0);
      }

      goto look_for_pid;
    got_mypid:
      if (reportdir[0])
      {
        strcpy (my_reportdir, reportdir);
        strcat(my_reportdir, "/clnt");
	if (jobname[0])
	  strcat(my_reportdir, jobname);
        sprintf(my_reportdir + strlen(my_reportdir),"%d", mypid);
        reportFILE = fopen(my_reportdir, "w");
      }
      if (ix != numforked)
      {
        if (verbose > 1)
          printline(stderr,"enquirerfn: enquirer %d: mismatch on numforked inherited= %d true= %d\n",mypid,numforked,ix);
        numforked = ix;
      }

      chshmp = supvr_workrP + numforked;       /*  address my supervisor_worker area */
      chshmp->ch_pid = mypid;
      /* get storage for connection_array, one per responder process  */
      ix = (   ( CONN_INFO_SIZE + sizeof(*responderproc_array) )
             * responderproccount ); /*  socket blocks */
#ifdef DYNAMIC_MSG_BUF         /*  malloc'd message buffers */
      jx = MSG_BLOK_SZ;
#if (DYNAMIC_MSG_BUF == 4096)  /*  page-aligned message buffers */
      jx = (jx+4095);          /*  ensure a page boundary  */
#endif                         /*  page-aligned message buffers */
      if (!(inmsgblok = (char *)malloc(jx))) /*  inbound message buffer  (one per process)  */
      {
        printline(stderr,"enquirerfn: enquirer %d: couldn't obtain storage size %d for inbound message buffer\n",mypid,jx);
        rc = 113;
        goto finish_enquirer;
      }
#if (DYNAMIC_MSG_BUF == 4096)  /*  page-aligned message buffers  */
      inmsgmall = inmsgblok;   /*  remember addr of mallocd area */
      inmsgblok = (char *)(((((long)inmsgmall)+4095)>>12)<<12); /*  page-aligned inbound message buffer */
#endif                         /*  page-aligned message buffers */

      if (!(outmsgblok = (char *)malloc(jx))) /*  outbound message buffer  (one per process)  */
      {
        printline(stderr,"enquirerfn: enquirer %d: couldn't obtain storage size %d for outbound message buffer\n",mypid,jx);
        rc = 113;
        goto finish_enquirer;
      }
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers */
      outmsgmall = outmsgblok; /* remember addr of mallocd area */
      outmsgblok = (char *)(((((long)outmsgmall)+4095)>>12)<<12); /*   page-aligned outbound message buffer */
#endif                        /*  page-aligned message buffers */
#endif                        /*  malloc'd message buffers */
      if (verbose > 1)
        printline(stderr,"enquirerfn: enquirer %d: inmsgblok= %p outmsgblok= %p \n",mypid,inmsgblok,outmsgblok);
      /*  initialize the outmsg buffer  */
      if (init_outmsg_buff(mypid, outmsgblok, enquirer_message_banner))
        goto finish_enquirer_worker;
#ifdef POLLSET
      ix += (sizeof(POLLEV_STRUCT) * (pollset_maxevents = responderproccount)); /* pollset events */
#endif
      if (socket_blocks_p = malloc(ix
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers */
              +4095
#endif                        /*  page-aligned message buffers */
         ))
      {
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
        socket_blocks_all = socket_blocks_p;  /*  remember addr of mallocd area */
        socket_blocks_p = (char *)(((((long)socket_blocks_all)+4095)>>12)<<12); /*  page-aligned socketblocks area */
#endif                        /*  page-aligned message buffers */

        /*  set our limit timer if specified by user */
        if (time_limit.it_value.tv_sec > 0)
          setitimer ( ITIMER_REAL, &time_limit, 0);

        memset(socket_blocks_p, 0, ix);
        connection_array = (struct connection_info *)socket_blocks_p;
        responderproc_array = (int *)(connection_array + responderproccount);
#ifdef POLLSET
        pollset_events = (POLLEV_STRUCT *)(responderproc_array + responderproccount); /* pollset events */
        if ((pollset_fd = POLLSE_CREATE(pollset_maxevents)) >= 0)
        {
#if (POLLSET & POLLSET_EDGE_TRIG)
          edge_triggered_event = 0;           /*  initialise flag to track occurrence of edge_triggered_events */
#else
          send_recv_oppy_count = 0;  /* count of number of connections that have a deferred opportunity for send or recv */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  */
#endif  /*  POLLSET */
        for (responderindex = 0; responderindex < responderproccount; responderindex++)
        {
          connection_array[responderindex].con_socket_desc = -1;  /*  indicate not connected */
          /******   no need to zero any fields, all memset earlier
                    connection_array[responderindex].con_numsent = connection_array[responderindex].con_numrecv
                    = connection_array[responderindex].con_nummsgr = connection_array[responderindex].con_blk_offset
                    = connection_array[responderindex].con_xsend_hwm = 0;
          ******/
          connection_array[responderindex].con_ranvalu = mypid;  /*  initial seed */
          /*  in case using our congruence generator, iterate it once to avoid initial clumpiness around low values */
          RAN_ANYVAL(ran_inputval_vala, ran_inputval_valc, connection_array[responderindex].con_ranvalu, 0 , 0);
        next_rand:
          responderproc_array[responderindex] = RAN_INPUTVAL(0 , responderproccount);  /*  random permutation */
          /*   check not already assigned
          **   note that the following loop around next_rand must terminate since the random domain is dense
          */
          for (ix = 0; ix < responderindex; ix++)
          {
            if (responderproc_array[responderindex] == responderproc_array[ix])
              goto next_rand;
          }
        }

        for (ix=0; ix<CONN_SOCK_SIZE; ix++) conn_by_socket[ix] = -1;  /*  init cache of connections  */
        max_socket_desc = connectioncount = sockclosecount = sendcount = recvcount = msgoutcount = msgincount = selectrc = 0;   /*  init counters  */
        enquirer_addrinfo_index = responder_addrinfo_index = ~0;

        /***********************************************************************************************
        ** we now schedule work as follows:
        ** if select() indicates data waiting to be received on a connection,
        ** then receive it
        ** if select() indicates a write would not wait
        ** then send next msg
        ** if there are any responder process(es) for which we have not yet connected,
        ** then choose one of these responder processes randomly and connect
        ** select()
        ** however, we modify this procedure to ensure that our total budget of sent msgs
        ** is distributed reasonably (not necessarily perfectly) over all responders:
        ** whenever:
        **        (1) there are responder process(es) for which we have not yet connected,
        **    and (2) we are considering sending on a connected socket whose select writelist bit is set
        **   then     send with probability ( 1 - ((sent_on_this_connection) / (total_numsent+(max_sendcount/6))) )
        **   this probability is designed to apply low resistance to sending on a new connection
        **   but increasing resistance as more messages are sent relative to the total max_sendcount
        *************************************************************************************************/
      schedule_work:
        if (   (signalled == SIGTERM)
            || (signalled == SIGALRM) /* time limit expired */
           )
          shutting_down = 1;  /*  commence graceful termination */

        skipped_a_send = 0;   /*  initially have not skipped a send */

        /***********************************************************************************************
        ** if select reported any send/recv opportunities, then take them
        ** check read lists first, then write lists.
        ** for edge-triggered pollset, always check all potentially eventful sockets -
        ** ignore the list from poll_wait and track events in our own flag edge_triggered_event
        *************************************************************************************************/
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  don't check retcode from poll_wait for edge-triggered */
        if (selectrc > 0)         /*  if we have something from select */
        {
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check all sockets for edge-triggered */
          edge_triggered_event = 0; /*  initialise indicator of whether we found an event */
        for (responderindex = 0; responderindex < responderproccount; responderindex++)
        {
          sd = connection_array[responderindex].con_socket_desc;
          if (   (sd >= 0)  /*  connected */
              && (connection_array[responderindex].con_nummsgr < connection_array[responderindex].con_nummsgw) /* msg expected */
             )
          {
#elif defined(POLLSET)
          for (evx = 0; evx < selectrc; evx++)
          {
            if (pollset_events[evx].POLLEV_TYP & POLLIN)   /* ready for reading */
            {
              pollin_oppys_count++;    /*   maintain count of pollin opportunities */
              sd =  pollset_events[evx].POLLEV_FD;
#else /* select  */
          int num_selected;          /*  number of fds returned from select */
          num_selected = selectrc;          /*  number of fds returned from select */
          fdwordp = (long *)&readlist;  /*  start searching for bits here */
          sd = 0;                       /*  first candidate socket */
          do
          {
            if (jx = *fdwordp)
            {                           /*  some socket set in this long-word  */
              for (kx = ((sizeof(long))*8); kx > 0; kx--)
              {
                if (jx & 0x01)
                {
                  /*  we must check sd again here in case highest lies inside this byte */
                  if (sd > max_socket_desc)
                    goto done_readlist;
#endif /* select     */
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  responderindex already set for edge-triggered */
                  responderindex = find_connection(sd, conn_by_socket, responderproccount, connection_array); /* locate connection */
                  if (responderindex >= 0)
                  {
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
                    /* receive a message on this connection */
                  recv_again:
                    /*  treat input msg len of 0 like CONNRESET */
                    memset(inmsgblok,0,MSG_BLOK_SZ);  /*  avoid misinterpreting content of incoming message */
                    /*  NOTE  - we include case of rc == 0 in following since that is what is returned if ENOTCONN   */
                    if ((send_recv_len = IPRECV_SD(sd, inmsgblok, MSG_BLOK_SZ, recv_flags ,responderindex)) <= 0)
                    {
                      rc = errno;
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
#ifdef DEBUG_NETRUSH
                      printline(((verbose > 2) ? stderr : 0),"enquirerfn: enquirer %d: responderindex %d sd %d recv error rc %d errno %d\n",mypid,responderindex,sd,send_recv_len,rc);
#endif  /* DEBUG_NETRUSH */
                      if  (   (send_recv_len == -1)
                           && ( (EAGAIN == rc) || (EWOULDBLOCK == rc) )
                          )
                        chshmp->ch_num_wdblck++;       /*  number of cases of EAGAIN and EWOULDBLOCK for edge-triggered */
                      else
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                      if (!(rc = handle_recv_error(sd,responderindex,conn_by_socket,responderproccount,connection_array,responderproc_array,rc,send_recv_len)))
                        goto recv_again;
                    }
                    else
                    {
                      rc = handle_recv_completion(chshmp ,sd,responderindex ,conn_by_socket ,responderproccount ,connection_array ,responderproc_array,send_recv_len);
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  remember successful event for edge-triggered */
                      if (!rc)
                        edge_triggered_event = 1; /*  note we found an event */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  remember successful event for edge-triggered */
                    }
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  don't check retcode from poll_wait for edge-triggered */
                  }
                  else
                  {
                      /*  if (verbose > 1)  */
                      printline(stderr,"enquirerfn: enquirer %d: can't find responderindex for socket %d\n",mypid,sd);
                  }
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
                  check_read_selects:
#ifndef POLLSET
                    if (--num_selected == 0)  /*  if we've seen all notifying sockets */
                      goto done_select_list;
                }
                jx >>= 1;    /*  examine next bit  ...            */
                sd++;        /*  ... corresponding to this socket */
              }
            }
            else sd += ((sizeof(long))*8);    /*  advance socket number past this empty long-word */
            fdwordp++;       /*  advance to next select list byte */
          } while (sd <= max_socket_desc); /*  for each connected socket */
#elif ! (POLLSET & POLLSET_EDGE_TRIG)  /* POLLSET  */
            ;
            }
          }
#else  /* POLLSET_EDGE_TRIG  */
            ;
            }
          }
#endif /* POLLSET_EDGE_TRIG  */
        done_readlist:

#ifdef POLLSET
          /*   for polling, we first revisit all deferred opportunities as noted in the connection entry,
          **   then check each event returned by poll
          */
          if (!shutting_down)
          {
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check all sockets for edge-triggered */
        for (responderindex = 0; responderindex < responderproccount; responderindex++)
        {
          sd = connection_array[responderindex].con_socket_desc;
          if (   (sd >= 0)  /*  connected */
#ifndef NO_CLIENT_PACING
              && ((excesssendcount = (connection_array[responderindex].con_nummsgw - connection_array[responderindex].con_nummsgr)) < XSEND_LIMIT)
#endif   /*  ALWAYS_TRY_WRITE  */
             )
          {
#else /* not edge-triggered */
          long send_recv_oppy_decrt; /* decrement count of number of connections that have a deferred opportunity for send or recv */
          send_recv_oppy_decrt = send_recv_oppy_count;
          responderindex = -1;   /*   start looking for deferred opportunities after here */
          for (evx = 0; ((send_recv_oppy_decrt > 0) || (evx < selectrc)); ((send_recv_oppy_decrt > 0) ? send_recv_oppy_decrt-- : evx++))
          {
            if (send_recv_oppy_decrt > 0)   /*  reconsider this opportunity */
            {
              /*   search forwards for next pollout, starting from next responderindex  */
              do
              {
                responderindex++;
                if (responderindex >= responderproccount)
                {
                  printline(stderr,"enquirerfn: enquirer %d: exhausted sds while searching for %d remaining opportunities\n",mypid,send_recv_oppy_decrt);
                  shutting_down = 1;   /*  commence graceful termination */
                  rc = 777;
                  goto finish_enquirer_worker;
                }
              }
              while (!(connection_array[responderindex].pollset_flags & POLLSET_SEND_OPPY));
              sd = connection_array[responderindex].con_socket_desc;
              connection_array[responderindex].pollset_flags &= ~POLLSET_SEND_OPPY;   /*  switch off to handle  */
              send_recv_oppy_count--;                   /*   keep count of these */
#ifdef DEBUG_NETRUSH
              printline(((verbose > 2) ? stderr : 0),"enquirerfn: enquirer %d: reconsidering deferred opportunity for sd %d , %d remaining opportunities\n",mypid,sd,send_recv_oppy_decrt);
#endif  /* DEBUG_NETRUSH */
              goto consider_responder_sd;
            }
            if (pollset_events[evx].POLLEV_TYP & POLLOUT)
            {
              sd =  pollset_events[evx].POLLEV_FD;
#endif /* not edge-triggered */
#else /* select  */
          fdwordp = (long *)&writelist;  /*  start searching for bits here */
          sd = 0;                        /*  first candidate socket */
          do
          {
            if (jx = *fdwordp)
            {                           /*  some socket set in this long-word  */
              for (kx = ((sizeof(long))*8); kx > 0; kx--)
              {
                if (jx & 0x01)
                {
                  /*  we must check sd again here in case highest lies inside this byte */
                  if (sd > max_socket_desc)
                    goto done_writelist;
#endif /* select  */
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  responderindex already set for edge-triggered */
                  responderindex = find_connection(sd, conn_by_socket, responderproccount, connection_array); /* locate connection */
                  if (responderindex >= 0)
                  {
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
                    /*   restrict excess of (msgw - msgr) to XSEND_LIMIT */
                  consider_responder_sd:
#if ( ! (POLLSET & POLLSET_EDGE_TRIG) )  /*  msg counts already checked for edge-triggered */
                    excesssendcount = (connection_array[responderindex].con_nummsgw - connection_array[responderindex].con_nummsgr); /* excess of (msgw - msgr) high-water-mark */
#endif /*  defined(POLLSET) && ( ! (POLLSET & POLLSET_EDGE_TRIG) )  msg counts already checked for edge-triggered */
#if ( ( ! defined(NO_CLIENT_PACING) ) && ( ! (POLLSET & POLLSET_EDGE_TRIG) ) ) /*  msg counts already checked for edge-triggered */
                    if (excesssendcount >= XSEND_LIMIT)
                    {
#if defined(POLLSET)
                      /*   since poll tells us only once about an opportunity to send and we are declining,
                      **   we must remember this send opportunity for next time
                      */
                      /*  the following test should always be true because of the above oneshot point, but check to be safe */
                      if (!(connection_array[responderindex].pollset_flags & POLLSET_SEND_OPPY))
                      {
                        connection_array[responderindex].pollset_flags |= POLLSET_SEND_OPPY;
                        send_recv_oppy_count++;                   /*   keep count of these */
#ifdef DEBUG_NETRUSH
                        printline(((verbose > 2) ? stderr : 0),"enquirerfn: enquirer %d: deferred opportunity for sd %d , %d remaining opportunities\n",mypid,sd,send_recv_oppy_count);
#endif  /* DEBUG_NETRUSH */
                      }
#endif /* POLLSET */
                      goto check_write_selects;
                    }
#endif /* CLIENT_PACING and not edge-triggered */
                    /*   if not all connections have been established,
                    **   apply probabilistic rule for sending
                    */
                    if ((connectioncount + sockclosecount) < responderproccount)
                    {
                      /*  random in range [ 0 , (total_numsent+(max_sendcount/6)) ] to decide on sending */
                      send_criterion = RAN_ANYVAL(ran_inputval_vala, ran_inputval_valc
                                                  ,connection_array[responderindex].con_ranvalu, 0, (sendcount+(expected_sendcount/6)));
                      if (connection_array[responderindex].con_nummsgw > send_criterion)
                      {
                        skipped_a_send = 1;
#if defined(POLLSET) && ( ! (POLLSET & POLLSET_EDGE_TRIG) )  /*  don't remember opportunities edge-triggered */
                        /*   since poll tells us only once about an opportunity to send and we are declining,
                        **   we must remember this send opportunity for next time
                        */
                        /*  the following test should always be true because of the above oneshot point, but check to be safe */
                        if (!(connection_array[responderindex].pollset_flags & POLLSET_SEND_OPPY))
                        {
                          connection_array[responderindex].pollset_flags |= POLLSET_SEND_OPPY;
                          send_recv_oppy_count++;                   /*   keep count of these */
#ifdef DEBUG_NETRUSH
                          printline(((verbose > 2) ? stderr : 0),"enquirerfn: enquirer %d: deferred opportunity for sd %d , %d remaining opportunities\n",mypid,sd,send_recv_oppy_count);
#endif  /* DEBUG_NETRUSH */
                        }
#endif /* POLLSET */
                        goto check_write_selects;
                      }
                    }
                    /* send a message on this connection */
                    sprintf(outmsgblok+MESSAGE_HDR_SIZE,"%lu",sendcount); /* crude message tracking */
                  send_again:
                    if ((send_recv_len = IPSEND_SD(sd, responderindex)) == -1)
                    {
                      rc = errno;
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                      if  ( (EAGAIN == rc) || (EWOULDBLOCK == rc) )
                        chshmp->ch_num_wdblck++;       /*  number of cases of EAGAIN and EWOULDBLOCK for edge-triggered */
                      else
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                      if (!(rc = handle_send_error(sd,responderindex,conn_by_socket,responderproccount,connection_array,responderproc_array,rc)))
                        goto send_again;
                    }
                    else
                    {
                      rc = handle_send_completion(chshmp ,sd,responderindex ,conn_by_socket ,responderproccount ,connection_array ,responderproc_array,send_recv_len);
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check whether event found */
                      if (!rc)
                        edge_triggered_event = 1; /*  note we found an event */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  check whether event found  */
                    }
                    /********************************************************************************
                    **  logic for termination of enquirer here
                    **   if we have used up all of our budget for max_sendcount, then start to shut down
                    ********************************************************************************/
                    if ((max_sendcount > 0) && (sendcount >= max_sendcount))
                      shutting_down = 1;   /*  commence graceful termination */
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  don't check retcode from poll_wait for edge-triggered */
                  }
                  else
                  {
                      /*  if (verbose > 1)  */
                      printline(stderr,"enquirerfn: enquirer %d: can't find responderindex for socket %d\n",mypid,sd);
                  }
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
                  check_write_selects:
#ifndef POLLSET
                    if (--num_selected == 0)  /*  if we've seen all notifying sockets */
                      goto done_select_list;
                }
                jx >>= 1;    /*  examine next bit  ...            */
                sd++;        /*  ... corresponding to this socket */
              }
            }
            else sd += ((sizeof(long))*8);    /*  advance socket number past this empty long-word */
            fdwordp++;       /*  advance to next select list byte */
          } while (sd <= max_socket_desc); /*  for each connected socket */

#elif ! (POLLSET & POLLSET_EDGE_TRIG)  /* POLLSET  */
            ;
            }
          }
#else  /* POLLSET_EDGE_TRIG  */
            ;
            }
          }
#endif /* POLLSET_EDGE_TRIG  */
#ifdef POLLSET
          }
          else
          {
            /********************************************************************
            **  logic for termination of enquirer here
            **   at this point, we know we are shutting down using pollset,
            **   so check pollin_oppys_count and finish if zero
            *********************************************************************/
            if
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check events for edge-triggered */
              (edge_triggered_event == 0) /*  no event */
#else  /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
              (pollin_oppys_count == 0)
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
              goto say_terminating;
          }
#endif /* POLLSET  */
        done_writelist:
        done_select_list: ;
#if ! (POLLSET & POLLSET_EDGE_TRIG)
        }   /*  end selectrc > 0 */
        /*  check for shutting down - if so, terminate only if timeout on select, to allow readlist to drain  */
        else
#else /* (POLLSET & POLLSET_EDGE_TRIG) */
          /*   for edge-triggered, even if shutting down, do not terminate if there are still events */
          if ((selectrc == 0) && (edge_triggered_event == 0))  /*  if we didn't find an event  */
          {
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
          /*************************************************************
          **  logic for termination of enquirer here
          *************************************************************/
          if (shutting_down)
          {
          say_terminating:
            printline(((verbose > 0) ? stderr : 0),"enquirerfn: enquirer %d: terminating because shutting down, signalled= %d"
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check events for edge-triggered */
                      " edge_triggered_event" /*  event */
#elif defined(POLLSET)
                      " pollin opportunities"
#else
                       " selectrc"
#endif /* select  */
                      "= %d"
                       "\n" ,mypid ,signalled
                       ,
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check events for edge-triggered */
                      edge_triggered_event /*  event */
#elif defined(POLLSET)
                      pollin_oppys_count
#else
                      selectrc
#endif /* select  */
                     );
            goto finish_enquirer_worker;
          }
#if (POLLSET & POLLSET_EDGE_TRIG)
          }
#endif /*  (POLLSET & POLLSET_EDGE_TRIG) */

      try_new_connection:
        /*   try opening a new connection only if:
        **         not shutting down
        **   AND   some responders are unconnected
        **   AND   EITHER we skipped a send
        **             OR select returned no ready sockets
        */
        if (    (!shutting_down)
            && (   (connectioncount
#ifndef AD_NAUSEAM
                /*  don't open any more after we have opened all responders at least once */
                   + sockclosecount
#endif
                   )           < responderproccount)
            && ( skipped_a_send ||
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check events for edge-triggered */
                 (edge_triggered_event == 0) /*  event */
#elif defined(POLLSET)
                 (pollin_oppys_count == 0)    /*   found no pollin opportunities */
#else /* select */
                 (!selectrc)
#endif /* select  */
               )
           )
        {
          /* connect to next responder process */
          responderindex = responderproc_array[connectioncount++];  /*  select responder randomly */
          if (responderindex < responderproccount)
          {
            /* fill in constant part of the socket structures with source and responder information */
            target_socket_p = &(connection_array[responderindex].othrend_socket);

            /*   here,  on first connection only, we may loop over all valid enquirer and responder sockets returned by getaddrinfo */
        try_specific_responder_socket_pair:
            if (connectioncount == 1) {
              if (responder_addrinfo_index == ~0)
                    responder_addrinfo_index = 0;
              else  {
                    responder_addrinfo_index++;
                    if (responder_addrinfo_index == maxrespondercount) {
                      printline(stderr,"enquirerfn: enquirer %d: cannot connect: exhausted all responder IP addresses for %s\n",mypid,enquirername);
                      goto finish_enquirer_worker;
                    }
              }
            }
        try_specific_enquirer_socket_pair:
            if (connectioncount == 1) {
              if (enquirer_addrinfo_index == ~0)
                    enquirer_addrinfo_index = 0;
              else  {
                    enquirer_addrinfo_index++;
                    if (enquirer_addrinfo_index == maxenquirercount) {
                      enquirer_addrinfo_index = ~0;
                      goto try_specific_responder_socket_pair;
                    }
              }
            }

            memcpy((void *)(&source_socket), (const void *)(enquire_addrP[enquirer_addrinfo_index].ai_addr), sizeof(source_socket));
            memcpy((void *)target_socket_p,  (const void *)(respond_addrP[responder_addrinfo_index].ai_addr), sizeof(*target_socket_p));

            /* fill in the source socket structure with enquirer port information
            ** add responderproccount + 1 to sport to ensure source and dest port are never equal to each other
            ** and source port never equal to any responder port,
            ** to handle case where source IPaddr = dest IPaddr
            ** 070206     furthermore, in order to ensure even distribution of linux bonding layer3+4 hashvalues,
            **            which are simple xor of (xor of source and dest port) with (xor of source and dest IP),
            **            when enquirerproccount and responderproccount are both even,
            **            we must alternate ascending and descending associations of enquirer and responder ports
            ** 110508     to allow for UDP,  responders assign different port for each connection,
            **            so enquirer port offsets start at (enquirerproccount * responderproccount)
            */
            current_port_num = (unsigned short)(lowestport + (((enquirerproccount + numforked + 1) * responderproccount) +
                                               ((numforked % 2) ? (responderproccount - responderindex) :
                                                   (responderindex + 1)
                                               ) /* 070206 */
                                              ));
            SET_PORTS_GENERAL(&source_socket , current_port_num);

            /* fill in the target socket structure with responder port information */
            current_port_num = (unsigned short)(lowestport + ( responderindex * enquirerproccount )
#ifdef UDP
	/*    ...    and for UDP, for each responder,  its sockets' port numbers are a sequence,  not a const */
						+ numforked
#endif /* UDP */
                                               );
            SET_PORTS_GENERAL(target_socket_p , current_port_num);

            /* obtain an Internet domain socket */
            if ((sd = socket(source_socket.sockaddr_u.opaque_sockaddr.sa_family, IPSOCKTYP, IPPROTCL)) >= 0)
            {
              connection_array[responderindex].con_socket_desc = sd;
              if (sd > max_socket_desc)
                max_socket_desc = sd;
              if (sd < CONN_SOCK_SIZE)
                conn_by_socket[sd] = responderindex;
#ifdef POLLSET
              if (!(rc = POLLSE_ADD(sd)))    /*   add to our pollset  */
              {
                connection_array[responderindex].pollset_flags = POLLSET_EV_POLLIN|POLLSET_EV_POLLOUT; /* remember registered events */
                    if (verbose > 1)
                      printline(stderr,"enquirerfn: enquirer %d: added socket %d to set\n",mypid,sd);
#endif /* POLLSET */
              /*  bind socket  */
              printline(((verbose > 0) ? stderr : 0),"%K%Kenquirerfn: enquirer %d: about to bind responder index %d socket desc %d to source addr %s port %hu target addr %s port %hu\n"
                        ,&source_socket ,hostBfr1 ,target_socket_p ,hostBfr2
                        ,mypid,responderindex,sd
                        ,hostBfr1,ntohs(source_socket.sin_port)
                        ,hostBfr2,ntohs(target_socket_p->sin_port)
                       );
              if ((rc = (bind (sd, (struct sockaddr *)(&source_socket), enquire_addrP[enquirer_addrinfo_index].ai_addrlen))) >= 0)
              {
                if (verbose > 1)
                {
                  if (rc = 4 , ((getsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char *)&send_recv_len, (unsigned int *)&rc)) >= 0))
                    printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d initial SO_SNDBUF %d\n",mypid,responderindex,sd,send_recv_len);
                  if (rc = 4 , ((getsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char *)&send_recv_len, (unsigned int *)&rc)) >= 0))
                    printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d initial SO_RCVBUF %d\n",mypid,responderindex,sd,send_recv_len);
                }
                /*  note - linux tcp (man page for tcp) claims to support RFC1323 without requiring anything to be set at tcp level */
#if defined(TCP_RFC1323) && ! defined(UDP)
                if (   (sosndbufsz || sorcvbufsz)
                    && ((setsockopt(sd, IPPROTCL, TCP_RFC1323, (char *)&on, sizeof(on))) < 0)
                   )
                {
                  rc = errno;
                  printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d TCP_RFC1323 failed errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                }
#endif
                if (   (sosndbufsz)
                    && ((setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char *)&sosndbufsz, sizeof(sosndbufsz))) < 0)
                   )
                {
                  rc = errno;
                  printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d SO_SNDBUF %d failed errno %d %s\n",mypid,responderindex,sd,sosndbufsz,rc,strerror(rc));
                }
                if (   (sorcvbufsz)
                    && ((setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char *)&sorcvbufsz, sizeof(sorcvbufsz))) < 0)
                   )
                {
                  rc = errno;
                  printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d SO_SCVBUF %d failed errno %d %s\n",mypid,responderindex,sd,sorcvbufsz,rc,strerror(rc));
                }
#if ! defined(UDP)
                if (   (tcp_nodelay)
                    && ((setsockopt(sd, IPPROTCL, TCP_NODELAY, (char *)&tcp_nodelay, sizeof(tcp_nodelay))) < 0)
                   )
                {
                  rc = errno;
                  printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d TCP_NODELAY failed errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                }
                /*  note - linux tcp (man page for tcp) claims to support RFC1323 without requiring anything to be set at tcp level */
#ifdef TCP_RFC1323
                if (   (tcp_rfc1323)
                    && ((setsockopt(sd, IPPROTCL, TCP_RFC1323, (char *)&tcp_rfc1323, sizeof(tcp_rfc1323))) < 0)
                   )
                {
                  rc = errno;
                  printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d TCP_RFC1323 failed errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                }
#endif
#endif /* ! defined(UDP) */

                /* connect to specified responder socket */
              do_connect:
               if ((rc = (IPCONNECT(sd, (struct sockaddr *)target_socket_p, SOCKADDR_SIZE))) >= 0)
                {
                  if (verbose && (sosndbufsz || sorcvbufsz))
                  {
                    if (rc = 4 , ((getsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char *)&send_recv_len, (unsigned int *)&rc)) >= 0))
                      printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d final SO_SNDBUF %d\n",mypid,responderindex,sd,send_recv_len);
                    if (rc = 4 , ((getsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char *)&send_recv_len, (unsigned int *)&rc)) >= 0))
                      printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d final SO_RCVBUF %d\n",mypid,responderindex,sd,send_recv_len);
                  }

#if   (POLLSET & POLLSET_EDGE_TRIG)
                  /* set the socket nonblocking */
                  if (rc = 1 , ioctl(sd, FIONBIO, &rc ) < 0 )
                  {
                    rc = errno;
                    printline(stderr,"enquirerfn: enquirer %d: ioctl FIONBIO on sd %d failed errno %d %s\n",mypid,sd,rc,strerror(rc));
                    goto finish_enquirer_worker;
                  }
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  */


                  /* send first message on this connection */
                send_first:


                  if ((send_recv_len = IPSEND_SD(sd, responderindex)) == -1)
                  {
                    if (!(rc = handle_send_error(sd,responderindex,conn_by_socket,responderproccount,connection_array,responderproc_array,errno)))
                      goto send_first;
                  }
                  else
                  {
                    rc = handle_send_completion(chshmp ,sd,responderindex ,conn_by_socket ,responderproccount ,connection_array ,responderproc_array,send_recv_len);
                  }
                }
                else
                {
                  rc = errno;
                  if (   (signalled == SIGTERM)
                      || (signalled == SIGALRM) /* time limit expired */
                     )
                    shutting_down = 1;   /*  commence graceful termination */
                  /*  for interrupts and transient failures, try again */
                  if ((rc == EINTR) || (rc == EAGAIN) || (rc == ETIMEDOUT))
                    goto do_connect;

                  /*    on first connection only, try next combination of enquirer and responder IP addresses */
                  if (connectioncount == 1) {
                    /*   it would probably be better to try next responder rather than next enquirer -
                    **   (because the bind of the enquirer socket was successful);
                    **   however that would require some convoluted looping logic.
                    **   looping on enquirers inner within responders outer is simpler
                    **   and ensures we will find a successful combination if one exists.
                    */
                    goto try_specific_enquirer_socket_pair;
                  }

                  printline(stderr,"enquirerfn: enquirer %d: responder index %d connect errno %d %s\n",mypid,responderindex,rc,strerror(rc));
                  shutting_down = 1;   /*  commence graceful termination */
                  goto setup_selects;
                }
              }
              else
              {
                rc = errno;
                printline(stderr,"%K%Kenquirerfn: enquirer %d: responder index %d source addr %s port %hu target addr %s port %hu bind errno %d %s\n"
                        ,&source_socket ,hostBfr1 ,target_socket_p ,hostBfr2
                        ,mypid,responderindex
                        ,hostBfr1,ntohs(source_socket.sin_port)
                        ,hostBfr2,ntohs(target_socket_p->sin_port)
                        ,rc,strerror(rc));
                if (rc == EADDRINUSE)  /*  in use from someone or somewhere else - continue without it */
                {
                  if ((rc = close_connection(sd, conn_by_socket, responderproccount, connection_array, responderproc_array)) >= 0)
                  {
                    printline(((verbose > 2) ? stderr : 0),"enquirerfn: enquirer %d: closed responder index %d socket desc %d EADDRINUSE\n",mypid,responderindex,sd);
                    rc = 0;
                    goto setup_selects;
                  }
                }

                /*    on first connection only, try next enquirer IP address */
                if (connectioncount == 1) {
                  goto try_specific_enquirer_socket_pair;
                }

                goto finish_enquirer_worker;
              }
#ifdef POLLSET
              }
              else
              {
                rc = errno;
                printline(stderr,"enquirerfn: enquirer %d: responderindex %d pollctl add sd %d errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                goto finish_enquirer_worker;
              }
#endif /* POLLSET */
            }
            else
            {
              rc = errno;
              printline(stderr,"enquirerfn: enquirer %d: responderindex %d socket errno %d %s\n",mypid,responderindex,rc,strerror(rc));
              goto finish_enquirer_worker;
            }
          }
          else
          {
            printline(stderr,"enquirerfn: enquirer %d: programming error - responderindex %d > responderproccount %d\n"
                      ,mypid,responderindex,responderproccount);
            rc = 181;
            goto finish_enquirer_worker;
          }
        }

      setup_selects:
        /*  SIGXCPU is for any experimenting - process will do something once and then clear it */
        if (signalled == SIGXCPU)
        {
          /*   try turning on TCP_NODELAY dynamically if it was on, else turn it off  */
          int temp_sd;
          /*  we should really maintain a shadow flag to track on or off but that clutters up the non-experimental code ... */
          tcp_nodelay = 1 - tcp_nodelay;    /* ... so simply toggle the user parm */
          for (jx = 0; jx < responderproccount; jx++)
          {
            if ((temp_sd = connection_array[jx].con_socket_desc) >= 0)
            {
              if ((setsockopt(temp_sd, IPPROTCL, TCP_NODELAY, (char *)&tcp_nodelay, sizeof(tcp_nodelay))) < 0)
              {
                rc = errno;
                printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d TCP_NODELAY failed errno %d %s\n",mypid,jx,temp_sd,rc,strerror(rc));
              }
              else printline(stderr,"enquirerfn: enquirer %d: responder index %d socket desc %d TCP_NODELAY now set to %d\n",mypid,jx,temp_sd,tcp_nodelay);
            }
          }
          signalled = 0;  /*  clear */
        }

#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
        /*    The following code is tedious.
        **    Find each case where registered events do not match desired events and issue commands to modify or delete/add.
        **    Semantics of MOD : new events replace existing
        */
        for (responderindex = 0; responderindex < responderproccount; responderindex++)
        {
          sd = connection_array[responderindex].con_socket_desc;
          if (sd >= 0)  /*  connected */
          {

            /*  On balance, it is probably better NOT to register / unregister POLLIN events -
            **  the overhead of doing so probably outweighs any benefit.
            **  Note that the objective of dynamic unregistering/registering is primarily to ensure that
            **       poll_wait calls do not return until there is some work to do,
            **       i.e. to reduce the number of poll_wait calls,
            **       and only secondarily to limit the work done by those poll_wait calls
            */
#if (POLLSET & POLLSET_DYNAMIC_POLLIN)
            /*  if message in expected but not registered, then register  */
            if (   (connection_array[responderindex].con_nummsgr < connection_array[responderindex].con_nummsgw)
                && (!(connection_array[responderindex].pollset_flags & POLLSET_EV_POLLIN))   /* not registered  */
               )
            {
              if (!(rc = POLLSE_MODF(sd, ( POLLIN|((connection_array[responderindex].pollset_flags & POLLSET_EV_POLLOUT) ? POLLOUT : 0) ))))
              {
                connection_array[responderindex].pollset_flags |= POLLSET_EV_POLLIN; /* remember registered POLLIN */
                if (verbose > 1)
                  printline(stderr,"enquirer: %d: responderindex %d registered POLLIN on sd %d\n",mypid,responderindex,sd);
                chshmp->ch_num_dynreg++;
              }
              else
              {
                rc = errno;
                printline(stderr,"enquirerfn: enquirer %d: responderindex %d pollctl register POLLIN on sd %d errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                goto finish_enquirer_worker;
              }
            }
            /*  if message in not expected but registered, then unregister  */
            else if (   (connection_array[responderindex].con_nummsgr >= connection_array[responderindex].con_nummsgw)
                     && (connection_array[responderindex].pollset_flags & POLLSET_EV_POLLIN)   /* registered  */
                    )
            {
              if (!(rc = POLLSE_DEL(sd)))    /*   remove from our pollset  */
              {
                connection_array[responderindex].pollset_flags &= ~POLLSET_EV_POLLIN; /* unregister POLLIN but preserve other flags temporarily for next test */
                if (verbose > 1)
                  printline(stderr,"enquirer: %d: responderindex %d  deleted socket %d from set\n",mypid,responderindex,sd);
                chshmp->ch_num_dynung++;
              }
              else
              {
                rc = errno;
                printline(stderr,"enquirerfn: enquirer %d: responderindex %d pollctl del sd %d errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                goto finish_enquirer_worker;
              }
              if (   (connection_array[responderindex].pollset_flags & POLLSET_EV_POLLOUT)  /* if we had been registered for POLLOUT */
                  && (rc = POLLSE_ADDF(sd, POLLOUT))
                 )
              {
                rc = errno;
                printline(stderr,"enquirerfn: enquirer %d: responderindex %d pollctl register POLLOUT on sd %d errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                goto finish_enquirer_worker;
              }
            }
#endif /* (POLLSET & POLLSET_DYNAMIC_POLLIN) */

            /*  if message out allowed but not registered, then register  */
            if (   (!shutting_down)
#ifndef NO_CLIENT_PACING
                && ((connection_array[responderindex].con_nummsgw - connection_array[responderindex].con_nummsgr) < XSEND_LIMIT)
#endif   /*  ALWAYS_TRY_WRITE  */
                && (!(connection_array[responderindex].pollset_flags & POLLSET_EV_POLLOUT))   /* not registered  */
               )
            {
              if (!(rc = POLLSE_MODF(sd, ( POLLOUT|((connection_array[responderindex].pollset_flags & POLLSET_EV_POLLIN) ? POLLIN : 0) ))))
              {
                connection_array[responderindex].pollset_flags |= POLLSET_EV_POLLOUT; /* remember registered POLLOUT */
                if (verbose > 1)
                  printline(stderr,"enquirer: %d: responderindex %d registered POLLOUT on sd %d\n",mypid,responderindex,sd);
                chshmp->ch_num_dynreg++;
              }
              else
              {
                rc = errno;
                printline(stderr,"enquirerfn: enquirer %d: responderindex %d pollctl register POLLOUT on sd %d errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                goto finish_enquirer_worker;
              }
            }
            /*  if message out not allowed but registered, then un-register  */
            else if (   (   (shutting_down)
#ifndef NO_CLIENT_PACING
                         || ((connection_array[responderindex].con_nummsgw - connection_array[responderindex].con_nummsgr) >= XSEND_LIMIT)
#endif   /*  ALWAYS_TRY_WRITE  */
                        )
                     && (connection_array[responderindex].pollset_flags & POLLSET_EV_POLLOUT)   /* registered  */
                    )
            {
#if (POLLSET & POLLSET_DYNAMIC_POLLIN)
              if (!(rc = POLLSE_DEL(sd)))    /*   remove from our pollset  */
              {
                connection_array[responderindex].pollset_flags &= ~POLLSET_EV_POLLOUT; /* unregister POLLOUT but preserve other flags temporarily for next test */
                if (verbose > 1)
                  printline(stderr,"enquirer: %d: responderindex %d  deleted socket %d from set\n",mypid,responderindex,sd);
                chshmp->ch_num_dynung++;
              }
              else
              {
                rc = errno;
                printline(stderr,"enquirerfn: enquirer %d: responderindex %d pollctl del sd %d errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                goto finish_enquirer_worker;
              }
              if (   (connection_array[responderindex].pollset_flags & POLLSET_EV_POLLIN)  /* if we had been registered for POLLIN */
                  && (rc = POLLSE_ADDF(sd, POLLIN))
                 )
              {
                rc = errno;
                printline(stderr,"enquirerfn: enquirer %d: responderindex %d pollctl register POLLIN on sd %d errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                goto finish_enquirer_worker;
              }
#else /* linux */
              /*   optimize for linux when no dynamic POLLIN so entry never deleted - use MOD to remove POLLOUT directly */
              if (!(rc = POLLSE_MODF(sd, POLLIN)))
              {
                connection_array[responderindex].pollset_flags &= ~POLLSET_EV_POLLOUT; /* unregister POLLOUT but preserve other flags temporarily for next test */
                if (verbose > 1)
                  printline(stderr,"enquirer: %d: responderindex %d  modified socket %d dropped POLLOUT\n",mypid,responderindex,sd);
                chshmp->ch_num_dynung++;
              }
              else
              {
                rc = errno;
                printline(stderr,"enquirerfn: enquirer %d: responderindex %d pollctl modify drop POLLIN on sd %d errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
                goto finish_enquirer_worker;
              }
#endif /* linux */
            }
          }

          else if (connection_array[responderindex].pollset_flags & (POLLSET_EV_POLLIN|POLLSET_EV_POLLOUT))  /*  registered  */
          {
            if (!(rc = POLLSE_DEL(sd)))    /*   remove from our pollset  */
            {
              connection_array[responderindex].pollset_flags &= ~(POLLSET_EV_POLLIN|POLLSET_EV_POLLOUT); /* now no registered events */
                    if (verbose > 1)
                      printline(stderr,"enquirer: %d: responderindex %d  deleted socket %d from set\n",mypid,responderindex,sd);
            }
            else
            {
              rc = errno;
              printline(stderr,"enquirerfn: enquirer %d: responderindex %d pollctl del sd %d errno %d %s\n",mypid,responderindex,sd,rc,strerror(rc));
              goto finish_enquirer_worker;
            }
          }
        }
#elif !defined(POLLSET)
        /*   set up the select lists  */
        Nfdsmsgs = max_socket_desc + 1;
        FD_ZERO(&readlist);
        FD_ZERO(&writelist);
        /***   FD_ZERO(ExceptList);   ***/
        for (responderindex = 0; responderindex < responderproccount; responderindex++)
        {
          sd = connection_array[responderindex].con_socket_desc;
          if (sd >= 0)  /*  connected */
          {
            /* do not expect to recv on a socket on which we have already received replies to all msgs sent */
            if (connection_array[responderindex].con_nummsgr < connection_array[responderindex].con_nummsgw)
              FD_SET(sd, &readlist);
            /*   if we are shutting down, don't look for sockets ready for sending */
            /*   in addition, throughput may be improved by avoiding testing sockets for write
            **   if we have reached our excess of (msgw - msgr) == XSEND_LIMIT
            **   Note that this is not necessarily beneficial
            **        as by doing so, if we happen to recv on this socket on this loop,
            **        we lose the opportunity to send next message immediately.
            **        However, testing shows it reduces enquirer CPU util enormously
            **       which may in turn permit higher throughput despite the lost opportunities for sends
            */
            if (   (!shutting_down)
#ifndef NO_CLIENT_PACING
                && ((connection_array[responderindex].con_nummsgw - connection_array[responderindex].con_nummsgr) < XSEND_LIMIT)
#endif   /*  ALWAYS_TRY_WRITE  */
               )
            {
              FD_SET(sd, &writelist);
            }
            /***   FD_SET(connection_array[responderindex].con_socket_desc, ExceptList);   ***/
          }
        }
#endif /* not POLLSET */
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  for edge-triggered, avoid the poll_wait if we found an edge-triggered event   */
        if (edge_triggered_event)
          goto schedule_work;
#endif /* for edge-triggered, avoid the poll_wait if we found an edge-triggered event   */
      do_select:
        /*   if we are shutting down, then increase timeout and don't look for sockets ready for sending */
        if (shutting_down)
          { TimeOut.tv_sec = 2;  TimeOut.tv_usec = 0; }
        else
          { TimeOut.tv_sec = 0;  TimeOut.tv_usec = 500000; }
        selectrc =
#ifdef POLLSET
           POLLEV_GET_OR_WAIT(pollset_fd, pollset_events, pollset_maxevents, ((TimeOut.tv_sec*1000)+(TimeOut.tv_usec/1000)));
#else /* select */
           select (Nfdsmsgs, &readlist, (shutting_down ? 0 : &writelist), /*** &ExceptList ***/ 0, &TimeOut);
#endif /* select  */
           chshmp->ch_num_selpol++;
#if defined(POLLSET) && ( ! (POLLSET & POLLSET_EDGE_TRIG) )
           pollin_oppys_count = 0;    /*   initialise count of pollin opportunities */
#endif /* POLLSET  */
           if (verbose > 3)
              printline(((verbose > 2) ? stderr : 0),"enquirerfn: enquirer %d: rc %d from "
#ifdef POLLSET
                        "poll"
#else /* select */
                        "select"
#endif /* select  */
                        " shtdn= %d\n",mypid,selectrc,shutting_down);
        if (selectrc == -1)
        {
          rc = errno;
          if (   (signalled == SIGTERM)
              || (signalled == SIGALRM) /* time limit expired */
             )
            shutting_down = 1;   /*  commence graceful termination */
          if ((rc == EINTR) || (rc == EAGAIN))
            goto do_select;
          printline(stderr,"enquirerfn: enquirer %d: select errno %d %s\n",mypid,rc,strerror(rc));
          shutting_down = 1;   /*  commence graceful termination */
        }
        goto schedule_work;
#ifdef POLLSET
      }
      else
      {
        rc = errno;
        printline(stderr,"enquirerfn: enquirer %d: couldn't create pollset size %d errno %d %s\n",mypid,pollset_maxevents,rc,strerror(rc));
        goto finish_enquirer;
      }
#endif  /*  POLLSET */
      }
      else
      {
        printline(stderr,"enquirerfn: enquirer %d: couldn't obtain storage size %d for socket structs\n",mypid,ix);
        rc = 111;
        goto finish_enquirer;
      }
    }
    else if (rc == -1)
    {
      rc = errno;
      printline(stderr,"enquirerfn: enquirer %d: fork failed errno %d %s\n",mypid,rc,strerror(rc));
      goto finish_enquirer_worker;
    }
    else        /*   non-zero retcd means I am supervisor and retcd is worker's pid */
    {
      rc = 0;       /*  if supervisor reaches here, set good rc  */
      pidarray[pidindex++] = otherpid;    /*   store pid of forked worker   */
      if (   (signalled == SIGTERM)
          || (signalled == SIGALRM) /* time limit expired */
         )
        goto finish_enquirer;
    }
  }               /*  end loop on forking workers   */

  /*  both supervisor and worker drop through to here */
 finish_enquirer:
    printing_ctl |= PRINT_ON;

  return rc;

 finish_enquirer_worker:
    printing_ctl |= PRINT_ON;

    get_finish_time(RUSAGE_SELF, &start_time, &finish_time, &accum_time_base);
    printline(((verbose > 2) ? stderr : 0),"enquirerfn: enquirer %d : sent %lu recv %lu msgw %lu msgr %lu "
               ": elapsed time %d.%" TVFORMAT "d end time %24.24s %06.6d\n user CPU %d.%06.6d, system CPU %d.%06.6d,\n vol contxt swchs %d, invol contxt swchs %d\n"
              ,mypid,sendcount,recvcount,msgoutcount,msgincount
              ,accum_time_base.tb_high, accum_time_base.tb_low
              ,ctime((time_t *)&(curtime.tv_sec)), curtime.tv_usec
              ,RUsage.ru_utime.tv_sec, RUsage.ru_utime.tv_usec
              ,RUsage.ru_stime.tv_sec, RUsage.ru_stime.tv_usec
              ,RUsage.ru_nvcsw, RUsage.ru_nivcsw
            );
  if (reportFILE)
    fclose (reportFILE);

  /*   at this point, supervisor can proceed to report results without needing to wait for me */
  chshmp->ch_flags |= CH_CHILD_FINISHED;


#ifdef SLEEP_NETRUSH_AT_END
#if (SLEEP_NETRUSH_AT_END == 1)
#undef SLEEP_NETRUSH_AT_END
#define SLEEP_NETRUSH_AT_END 300
#endif
  sleep(SLEEP_NETRUSH_AT_END);
#endif


 enquirer_shutdown_socks:
#ifdef CLOSONOCLIENT
    /*  actually seems safer to leave sockets open ... */
#else
  for (responderindex = 0; responderindex < responderproccount; responderindex++)
  {
     if ((sd = connection_array[responderindex].con_socket_desc) >= 0)  /*  if connected */
     {
#if 0
       /*   the following causes some kind of race resulting in processes exiting early */
#ifndef AD_NAUSEAM
       /* send a one-byte indicator to notify the responder that we are terminating
       ** first enable non-blocking IO for socket since this is not crucial
       */
       ioctl(sd,FIONBIO,&on);
       send(sd, stream_eof, 1, 0);
#endif
#endif
       /* close(sd); */
       shutdown(sd,SHUT_RDWR);
     }
  }
#endif
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
  if (socket_blocks_all)   free(socket_blocks_all);
#else
  if (socket_blocks_p)   free(socket_blocks_p);
#endif                        /*  page-aligned message buffers */
#ifdef DYNAMIC_MSG_BUF        /*  malloc'd message buffers */
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
      if (inmsgmall)  free(inmsgmall);  /*   inbound message buffer  */
      if (outmsgmall) free(outmsgmall); /*  outbound message buffer  */
#else                         /*  not page-aligned message buffers */
      if (inmsgblok)  free(inmsgblok);  /*   inbound message buffer  */
      if (outmsgblok) free(outmsgblok); /*  outbound message buffer  */
#endif                        /*  not page-aligned message buffers */
#endif                        /*  malloc'd message buffers */

#ifndef CLOSONOCLIENT    /*  actually seems safer to leave the darned thing open ... */
#endif
#ifdef POLLSET
        close(pollset_fd);
#endif  /*  POLLSET */
 /*  exit here seems to result in lost output so return to main which will return again to OS
     exit (rc);
  */
 return rc;
}

struct accept_parm_obj  /*  pointers to parameters for accept_connections */
{
  int listensd;    /*  listener socket fd  */
  struct connection_info *connection_array;  /*  array of all connections */
  /*  values returned  when called synchronously  */
  int accept_rc;     /*  return code  -  >= 0 <==> ok and msgsock has socketdescr, < 0 <==> not ok and msgsock has errno */
  int msgsock;       /*  returned msgsock or errno (see above)  when called synchronously  */
};

int add_responder_connection(struct connection_info *connection_array
                         ,int msgsock
                         ,short *conn_by_socket
                         ,long *connectioncount_p
                         ,int *max_socket_desc_p
                         )
     /*   add a new msgsock to the responder connection array if room
      *   choose the first unused entry in the connection array or fail if there are none
     */
#define connectioncount (*connectioncount_p)
#define max_socket_desc (*max_socket_desc_p)
{
  int rc;
  long connindex = 0; /*  index of unused entry */

    while ( (connection_array[connindex].con_socket_desc != -1) && (connindex < enquirerproccount) ) {
          connindex++;
    }
    if (rc = (connindex == enquirerproccount))  /*  exceeded what we can accommodate */
    {
      printline(stderr,"add_responder_connection: responder %d: conn %ld exceeds what we can accommodate,  number currently connected= %ld\n"
                ,getpid(),connindex,connectioncount);
      close(msgsock);
    }
    else
    {
#ifdef POLLSET
      if (!(rc = POLLSE_ADD(msgsock)))    /*   add to our pollset  */
      {
         connection_array[connindex].pollset_flags = POLLSET_EV_POLLIN|POLLSET_EV_POLLOUT; /* remember registered events */
               /*   if (verbose > 1)  */
                      printline(stderr,"add_responder_connection: responder %d: added connection connindex %ld with msgsock %d to set\n",mypid,connindex,msgsock);
#else  /* not POLLSET */
        /*  if (verbose > 1)  */
                      printline(stderr,"add_responder_connection: responder %d: added connection connindex %ld with msgsock %d\n",mypid,connindex,msgsock);
#endif /* POLLSET */
      connection_array[connindex].con_socket_desc = msgsock;
      if (msgsock > max_socket_desc)
        max_socket_desc = msgsock;
      if (msgsock < CONN_SOCK_SIZE)
        conn_by_socket[msgsock] = connindex;
      connectioncount++;
#ifdef POLLSET
      }
      else
      {
        rc = errno;
        printline(stderr,"add_responder_connection: responder %d: pollctl add sd %d errno %d %s\n",mypid,msgsock,rc,strerror(rc));
        rc = -(rc+6144);  /*  ensure negative */
      }
#endif /* POLLSET */
    }
    return rc;
}
#undef connectioncount
#undef max_socket_desc

void *accept_connections(void *t_parm_p)
/*  function to accept incoming connections
**  for synchronous IO, called when needed (connection-request waiting)
**  loop of:
**             blocking accept
**  (We don't update the main thread's connection table here since we would have to serialize)
**  don't print directly to stderr/reportfile, instead print to buffer
*/
{
  int rc, accept_rc;
  struct accept_parm_obj *accept_parm_p;
  int listensd, msgsock;
  struct sockaddr_general socket_name;
  int send_recv_len;                       /*  value of some socket parm */
  int sprintf_len;                         /* length of sprintfd message */
  socklen_t socket_name_length;

  accept_parm_p = (struct accept_parm_obj *)t_parm_p;
  listensd = accept_parm_p->listensd;

 listen_again:
  if ((accept_con_pbuf_cur - accept_con_pbuf_start) > ACCEPT_CON_PBUF_LEN/2) /* if used more than half buffer */
    accept_con_pbuf_cur = accept_con_pbuf_start;                             /* then overwrite */
 accept_rc =  msgsock = (accept(listensd, 0, 0));             /*  get a connection */
  accept_parm_p->accept_rc = accept_parm_p->msgsock = msgsock;
  if (accept_rc >= 0)  /*  got a connection */
  {
    if (verbose > 1)
    {
      if (rc = 4 , ((getsockopt(msgsock, SOL_SOCKET, SO_SNDBUF, (char *)&send_recv_len, (unsigned int *)&rc)) >= 0))
      {
        if ((sprintf_len = sprintf(accept_con_pbuf_cur,"accept_connections: responder %d: conn index %ld socket desc %d initial SO_SNDBUF %d\n",mypid,connectioncount,msgsock,send_recv_len)) > 0) accept_con_pbuf_cur += sprintf_len;
      }
      if (rc = 4 , ((getsockopt(msgsock, SOL_SOCKET, SO_RCVBUF, (char *)&send_recv_len, (unsigned int *)&rc)) >= 0))
        if ((sprintf_len = sprintf(accept_con_pbuf_cur,"accept_connections: responder %d: conn index %ld socket desc %d initial SO_RCVBUF %d\n",mypid,connectioncount,msgsock,send_recv_len)) > 0) accept_con_pbuf_cur += sprintf_len;
    }
                /*  note - linux tcp (man page for tcp) claims to support RFC1323 without requiring anything to be set at tcp level */
#ifdef TCP_RFC1323
    if (   (sosndbufsz || sorcvbufsz)
        && ((setsockopt(msgsock, IPPROTCL, TCP_RFC1323, (char *)&on, sizeof(on))) < 0)
       )
    {
      rc = errno;
      if ((sprintf_len = sprintf(accept_con_pbuf_cur,"accept_connections: responder %d: conn index %ld socket desc %d TCP_RFC1323 failed errno %d %s\n",mypid,connectioncount,msgsock,rc,strerror(rc))) > 0) accept_con_pbuf_cur += sprintf_len;
    }
#endif
    if (   (sosndbufsz)
        && ((setsockopt(msgsock, SOL_SOCKET, SO_SNDBUF, (char *)&sosndbufsz, sizeof(sosndbufsz))) < 0)
       )
    {
      rc = errno;
      if ((sprintf_len = sprintf(accept_con_pbuf_cur,"accept_connections: responder %d: conn index %ld socket desc %d SO_SNDBUF %d failed errno %d %s\n",mypid,connectioncount,msgsock,sosndbufsz,rc,strerror(rc))) > 0) accept_con_pbuf_cur += sprintf_len;
    }
    if (   (sorcvbufsz)
        && ((setsockopt(msgsock, SOL_SOCKET, SO_RCVBUF, (char *)&sorcvbufsz, sizeof(sorcvbufsz))) < 0)
       )
    {
      rc = errno;
      if ((sprintf_len = sprintf(accept_con_pbuf_cur,"accept_connections: responder %d: conn index %ld socket desc %d SO_SCVBUF %d failed errno %d %s\n",mypid,connectioncount,msgsock,sorcvbufsz,rc,strerror(rc))) > 0) accept_con_pbuf_cur += sprintf_len;
    }
    if (verbose && (sosndbufsz || sorcvbufsz))
    {
      if (rc = 4 , ((getsockopt(msgsock, SOL_SOCKET, SO_SNDBUF, (char *)&send_recv_len, (unsigned int *)&rc)) >= 0))
        if ((sprintf_len = sprintf(accept_con_pbuf_cur,"accept_connections: responder %d: conn index %ld socket desc %d final SO_SNDBUF %d\n",mypid,connectioncount,msgsock,send_recv_len)) > 0) accept_con_pbuf_cur += sprintf_len;
      if (rc = 4 , ((getsockopt(msgsock, SOL_SOCKET, SO_RCVBUF, (char *)&send_recv_len, (unsigned int *)&rc)) >= 0))
        if ((sprintf_len = sprintf(accept_con_pbuf_cur,"accept_connections: responder %d: conn index %ld socket desc %d final SO_RCVBUF %d\n",mypid,connectioncount,msgsock,send_recv_len)) > 0) accept_con_pbuf_cur += sprintf_len;
    }

    if (   (tcp_nodelay)
        && ((setsockopt(msgsock, IPPROTCL, TCP_NODELAY, (char *)&tcp_nodelay, sizeof(tcp_nodelay))) < 0)
       )
    {
      rc = errno;
      if ((sprintf_len = sprintf(accept_con_pbuf_cur,"accept_connections: responder %d: conn index %ld socket desc %d TCP_NODELAY failed errno %d %s\n",mypid,connectioncount,msgsock,rc,strerror(rc))) > 0) accept_con_pbuf_cur += sprintf_len;
    }
                /*  note - linux tcp (man page for tcp) claims to support RFC1323 without requiring anything to be set at tcp level */
#ifdef TCP_RFC1323
    if (   (tcp_rfc1323)
        && ((setsockopt(msgsock, IPPROTCL, TCP_RFC1323, (char *)&tcp_rfc1323, sizeof(tcp_rfc1323))) < 0)
       )
    {
      rc = errno;
      if ((sprintf_len = sprintf(accept_con_pbuf_cur
                  ,"accept_connections: responder %d: conn index %ld socket desc %d TCP_RFC1323 failed errno %d %s\n"
                  ,mypid,connectioncount,msgsock,rc,strerror(rc))) > 0) {
			accept_con_pbuf_cur += sprintf_len;
      }
    }
#endif
    if (   (   (socket_name_length = sizeof(struct sockaddr_general)) ,
               ((rc = (getsockname (msgsock, (struct sockaddr *)&socket_name, &socket_name_length))) >= 0)
           )
       )
    {
      RECONCILE_PORTS_GENERAL(&socket_name);                          /*  reconcile our copy of port number */
  
      if (getnameinfo((struct sockaddr *)(&socket_name), sizeof(struct sockaddr_general), hostBfr1, NI_MAXHOST, 0, 0, NAMEINFO_FLAGS) != 0) {
        fprintf(stderr,"%s accept_connections:  getnameinfo of socket_addr at 0x%p family %hu port %hu reports errno %d\n",command_nameP
                   ,(char *)(&socket_name) ,socket_name.sockaddr_u.opaque_sockaddr.sa_family ,socket_name.sin_port, errno);
        hostBfr1[0] = '\0';
      }
      if ((sprintf_len = sprintf(accept_con_pbuf_cur
                  ,"accept_connections: responder %d: num already connected %ld socket desc %d addr %s port %hu\n"
                  ,mypid,connectioncount,msgsock
                  ,hostBfr1,ntohs(socket_name.sin_port))) > 0) {
			  accept_con_pbuf_cur += sprintf_len;
      }
    }
  }
  else
  {
    /*  else - up to caller to report error, we just pass it back */
    msgsock = errno;   /*  best way to indicate an error ... */
    if (msgsock > 0)
      msgsock = - msgsock;
    else if (msgsock == 0)
      msgsock = -1;       /*  ...  and ensure negative */
    accept_parm_p->msgsock = errno;
  }

#if   (POLLSET & POLLSET_EDGE_TRIG)
  /* set the socket nonblocking */
  if (   (accept_rc >= 0)  /*  got a connection */
      && ( rc = 1 , ( ioctl(msgsock, FIONBIO, &rc ) < 0 ) )  /*  try to make non-blocking .. */
     )
  {
    rc = errno;
    if ((sprintf_len = sprintf(accept_con_pbuf_cur,"accept_connections: enquirer %d: ioctl FIONBIO on msgsock %d failed errno %d %s\n",mypid,msgsock,rc,strerror(rc))) > 0) accept_con_pbuf_cur += sprintf_len;
    goto exit_thread;
  }
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  */

 exit_thread:

  return (void *)0;
}

#ifdef MSG_OUTPUT_TO_FILE
/*  write linefeed-delimited output to file when a line spans either previous or current inmsgblok
**  return >= 0 for success or mild exception,  <0 for fatal disaster
*/
int handle_line_spanned_msg(struct connection_info *conninfoP , int msg_output_fd , char **inmsgmallP , char **inmsgblokP , int send_recv_len)
{
    int rc = -9;  /*  note that rc will be set in one or other of the two if blocks below  -  caller guarantees one of them will be taken */
    char *local_inmsgblok;
    char *local_inmsgmall;
    int line_spanned_offset;   /* offset in current inmsgblok of next char to be output to file */

        local_inmsgmall = *inmsgmallP; /* local copy of addr of current inmsgmall area */
        local_inmsgblok = *inmsgblokP; /* local copy of addr of current inmsgblok buffer */
        /*  first write the pending residual characters,
        **  then scan the current inmsgblok
        */
        if (    (0 != conninfoP->line_spanned_msgblok) /*  there is a residual */
             && (conninfoP->line_spanned_send_recv_len > conninfoP->line_spanned_offset)
           ) {
            rc = write(msg_output_fd , (conninfoP->line_spanned_msgblok+conninfoP->line_spanned_offset)
                                , (conninfoP->line_spanned_send_recv_len-conninfoP->line_spanned_offset));
        }
        /*  this residual msgblok is now exhausted and can be either reused or freed depending on whether still needed */

        line_spanned_offset = send_recv_len - 1;                        /*  offset of last valid character */
        if ('\n' != *(local_inmsgblok + line_spanned_offset)) {               /*  line spans current */
            /*  scan back from end to start to find last linefeed  */
            do {
                line_spanned_offset--;
            } while ( (line_spanned_offset >= 0) && ('\n' != *(local_inmsgblok + line_spanned_offset)) );

            if (    (line_spanned_offset < 0)
                 && (0 != conninfoP->line_spanned_msgblok)
               )  {
                /*   unfortunate case  -
                **   there was a residual *and* no linefeed anywhere within this current inmsgblok -
                **   this implies the current line spans more than two bloks!
                **   in which case it is not possible to guarantee no interleaving
                */
                    printline(stderr,"responderfn: responder %d: current line spans more than two tcp recv msgs, cannot guarantee no interleaving in msg %d %.*s\n"
                                                  ,mypid,conninfoP->con_numrecv,send_recv_len,local_inmsgblok);
                    line_spanned_offset = (send_recv_len - 1);   /*   no choice other than to write the entire blok */
            }
        }

        /*  line_spanned_offset is now the offset of the last character to write from current inmsgblok */
        if ( ++line_spanned_offset > 0) {
            rc = write(msg_output_fd , local_inmsgblok , line_spanned_offset);
        }

        /*  line_spanned_offset is now the length written,
        **    i.e. the offset of the character following last written from current inmsgblok
        **  now update state in connection block and dispose of old residual if any
        */
        if (line_spanned_offset < send_recv_len) {    /*  some of current blok not written ==>> becomes new residual */
            if (0 != conninfoP->line_spanned_msgblok) { /*  there is a (previous) residual */
                *inmsgmallP = conninfoP->line_spanned_msgmall; /* so reuse it as new inmsgblock */
                *inmsgblokP = conninfoP->line_spanned_msgblok; /* so reuse it as new inmsgblock */
            } else {  /*  obtain a new current inmsgblok  */
                /*  we can assume DYNAMIC_MSG_BUF == 4096 - enforced earlier */
                if (!(*inmsgmallP = (char *)malloc(MSG_BLOK_SZ+4095))) {
                    printline(stderr,"responderfn: responder %d: couldn't obtain storage size %d for inbound message buffer\n",mypid,(MSG_BLOK_SZ+4095));
                    rc = -1;
                } else {
                    *inmsgblokP = (char *)(((((long)(*inmsgmallP))+4095)>>12)<<12); /*  page-aligned inbound message buffer */
                }
            }
            conninfoP->line_spanned_msgmall = local_inmsgmall;   /* and set current as residual  */
            conninfoP->line_spanned_msgblok = local_inmsgblok;   /*     set current as residual  */
            conninfoP->line_spanned_offset = line_spanned_offset; /* new residual offset */
            conninfoP->line_spanned_send_recv_len = send_recv_len; /* new residual length */
        } else {                                     /*  there is now no (new) residual */
            if (0 != conninfoP->line_spanned_msgblok) { /*  there is a (previous) residual */
                free(conninfoP->line_spanned_msgmall);
                conninfoP->line_spanned_msgmall = 0;   /* no residual  */
                conninfoP->line_spanned_msgblok = 0;   /* no residual  */
                conninfoP->line_spanned_offset = 0;
                conninfoP->line_spanned_send_recv_len = 0;
            }
        }
                
        return rc;
}
#endif  /* MSG_OUTPUT_TO_FILE */

/*  responderfn - all processing performed by the responder - called as single supervisor process  */
int responderfn()
{
  int rc;
  int selectrc;          /*  return code from select */
  int sd, length;
  struct sockaddr_general responder_socket;
  int   msgsock;
  struct sockaddr_general *socket_name_p;
  socklen_t socket_name_length;

  int errno_l = 0;                     /*  local copy of errno  */
  struct connection_info *connection_array;  /*  array of all connections,
                                                 ordered by connection index, including socket descriptors  */
  int Imsupervisor, otherpid;
  char *socket_blocks_p;

#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
  char *socket_blocks_all;    /*  mallocd socketblocks area     */
#endif                        /*  page-aligned message buffers */

  long numforked, ix, jx, kx, lx, px, cx;
  long connindex;               /*  connection index, which indexes connection_array  */

  long no_work_done;   /*  indicates no work done on current iteration of select loop */

  /*   the following are parms for select */
  int on;
  int Nfdsmsgs;
  int select_list_size;
  /*   int max_socket_desc;   now global */
  fd_set readlist;
  fd_set writelist;
  /***   fd_set ExceptList;   ***/
  struct timeval TimeOut = { 0 , 500000 }; /* set timeout to 0.5 sec */

  int send_recv_len;                       /*  length of IP packet msg sent or received */

  char *charptr;
  long *fdwordp;   /*  pointer to a select fdlist word, i.e. a sequence of bits representing fds
                   **  note that fd numbers ascend from low to high within a word of type long.
                   **        Remarkably, this is true even though the size of a long differs between 32-bit and 64-bit -
                   **        somehow the function "knows" what size my application is compiled to use (!)
                   **        so, e.g.    -       in 32-bit, fd 5 is located here      0x2ff1bd88:  00000020
                   **        so, e.g.    -  but  in 64-bit, fd 5 is located here      0x0fffffffffff8a70:  00000000 00000020
                   */
  short conn_by_socket[CONN_SOCK_SIZE]; /* cache of responder indexes for lowest socket descriptors
                                        ** must be signed since we use -1 to indicate none
                                        */
  char my_reportdir[256];
#ifdef MSG_OUTPUT_TO_FILE
  char my_msg_output_path[256];
  int  my_msg_output_fd = -1;  /*  write received msgs here if specified */
#endif  /* MSG_OUTPUT_TO_FILE */
  struct supvr_workr_common *chshmp;         /*  pointer to worker shared-mem area */

  struct accept_parm_obj accept_parm;
#ifdef POLLSET
        POLLEV_STRUCT *pollset_events;    /*   ->  array of "completed" pollset events */
        long evx;                         /*   index into pollset_events array */
#if (POLLSET & POLLSET_EDGE_TRIG)
        long edge_triggered_event;        /*   flag to track occurrence of edge_triggered_events */
#else
        long send_recv_oppy_count;        /*   count of number of connections that have a deferred opportunity for send or recv */
        long pollin_oppys_count;          /*   count of number of pollin opportunities from most recent select - for deciding whether to terminate */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  */
#endif /* POLLSET */
        struct itimerval resume_time_limit;     /*  limits time for job   -  save and restore remaining interval         */
#ifdef UDP
        int already_received_msg;             /*  have we received message from this otherend before ? */
        unsigned int numenquirers_received_msg = 0;  /*  total number of enquirers who have sent us a message  */
        unsigned int num_consctv_sel_tmouts;  /*  number of consecutive select timeouts - to decide on responder termination */
#endif /*  UDP */
/*     for addrinfo - IPV6 enablement */
       unsigned int responder_addrinfo_index;  /*  index into respond_addrP array of the IP address to use  */

  shutting_down = 0;

  /*   fork worker processes */
  for (numforked = 0; numforked < responderproccount; numforked++)
  {
    if ( ! ( rc = Imsupervisor = otherpid = fork() ) ) /* fork worker */
    {
      /*  rc == 0 - worker  */
      mypid = getpid(); /* my pid */
      socket_blocks_p = 0;

#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
     socket_blocks_all = 0;   /*  mallocd socketblocks area     */
#endif                        /*  page-aligned message buffers  */

      /*  we depend on value of numforked in this worker being same as it was in supervisor whern forking us
      **  (for assigning socket port number, see reference to numforked below)
      **  you might think that worker must inherit this value since fork is supposed to create
      **  an exact memory image.  However, on some machines, numforked may be in a data cache
      **  resulting in stale value inherited.   So verify from pidarray until consistent.
      **  note supervisor stores my pid in pidarray and flushes cache; we also flush cache before referencing,
      **       although in our case the flush is for its side-effect of invalidation
      */
    look_for_pid:
      for (ix = 0; (pidarray && (
                    jx = pidarray[ix])); ix++)
      {
        if (jx == mypid)  goto got_mypid;
      }
    /*  note : worker does not depend on time_limit alarm */
      setitimer ( ITIMER_REAL, &sleeptime, 0);
      sigprocmask(SIG_BLOCK,&blockalarmask,&tempmask); /*  temporarily block signal ALRM... */
      while (!signalled)                      /*  check we have received the correct signal */
          sigsuspend(&tempmask);                   /*   wait for signal             */
      sigprocmask(SIG_SETMASK,&tempmask,0);       /*  normal mask allows ALRM */
      signalled = 0;                              /*   reset indicator        */
      goto look_for_pid;

    got_mypid:
      if (reportdir[0])
      {
        strcpy (my_reportdir, reportdir);
        strcat(my_reportdir, "/srvr");
	if (jobname[0])
	  strcat(my_reportdir, jobname);
        sprintf(my_reportdir + strlen(my_reportdir),"%d", mypid);
        reportFILE = fopen(my_reportdir, "w");
      }
#ifdef MSG_OUTPUT_TO_FILE
      if (msg_output_path[0])
      {
        strcpy (my_msg_output_path, msg_output_path);
        strcat(my_msg_output_path, ".srvr");
	if (jobname[0])
	  strcat(my_msg_output_path, jobname);
        sprintf(my_msg_output_path + strlen(my_msg_output_path),"%d", mypid);
        my_msg_output_fd = open(my_msg_output_path, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
        if (my_msg_output_fd < 0 ) {
           printline(stderr,"responderfn: could not open %s errno %d\n",my_msg_output_path, errno);
        }
      }
#endif  /* MSG_OUTPUT_TO_FILE */
      if (ix != numforked)
      {
        if (verbose > 1)
          printline(stderr,"responderfn: responder %d: mismatch on numforked inherited= %d true= %d\n",mypid,numforked,ix);
        numforked = ix;
      }

      chshmp = supvr_workrP + numforked;       /*  address my supervisor_worker area */
      chshmp->ch_pid = mypid;
      /* get storage for all socket descs, one per enquirer process (we accept only one per enquirer) */
      px = ix = ( ( CONN_INFO_SIZE
#ifdef UDP
                  +  SOCKADDR_SIZE  /*  to hold array of socket structs for the other ends */
#endif /*  UDP  */
                  ) * enquirerproccount ); /* socket blocks */
#ifdef DYNAMIC_MSG_BUF        /*  malloc'd message buffers */
      jx = MSG_BLOK_SZ;
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers */
      jx = (jx+4095);         /*  ensure a page boundary  */
#endif                        /*  page-aligned message buffers */
      if (!(inmsgblok = (char *)malloc(jx))) /*  inbound message buffer  (one per process)  */
      {
        printline(stderr,"responderfn: responder %d: couldn't obtain storage size %d for inbound message buffer\n",mypid,jx);
        rc = 113;
        goto finish_responder;
      }
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
      inmsgmall = inmsgblok;  /*  remember addr of mallocd area */
      inmsgblok = (char *)(((((long)inmsgmall)+4095)>>12)<<12); /*  page-aligned inbound message buffer */
#endif                        /*  page-aligned message buffers */

      if (!(outmsgblok = (char *)malloc(jx))) /*  outbound message buffer  (one per process)  */
      {
        printline(stderr,"responderfn: responder %d: couldn't obtain storage size %d for outbound message buffer\n",mypid,jx);
        rc = 113;
        goto finish_responder;
      }
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers */
      outmsgmall = outmsgblok; /* remember addr of mallocd area */
      outmsgblok = (char *)(((((long)outmsgmall)+4095)>>12)<<12); /*   page-aligned outbound message buffer */
#endif                        /*  page-aligned message buffers */
#endif                        /*  malloc'd message buffers */
      if (verbose > 1)
        printline(stderr,"responderfn: responder %d: inmsgblok= %p outmsgblok= %p \n",mypid,inmsgblok,outmsgblok);
      /*  initialize the outmsg buffer  */
      if (init_outmsg_buff(mypid, outmsgblok, responder_message_banner))
        goto finish_responder_worker;
#ifdef POLLSET
      ix += (sizeof(POLLEV_STRUCT) * (pollset_maxevents = (enquirerproccount+1))); /* pollset events - the +1 is for the listener */
#endif
      lx = ix;     ix += ACCEPT_CON_PBUF_LEN;           /*   plus accept_connections print buffer */
      if (socket_blocks_p = malloc(ix
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers */
              +4095
#endif                        /*  page-aligned message buffers */
         ))
      {
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
        socket_blocks_all = socket_blocks_p;  /*  remember addr of mallocd area */
        socket_blocks_p = (char *)(((((long)socket_blocks_all)+4095)>>12)<<12); /*  page-aligned socketblocks area */
#endif                        /*  page-aligned message buffers */

        /*  set our limit timer if specified by user */
        if (time_limit.it_value.tv_sec > 0)
          setitimer ( ITIMER_REAL, &time_limit, 0);

        memset(socket_blocks_p, 0, ix);
        accept_con_pbuf_cur = accept_con_pbuf_start = socket_blocks_p + lx; /*  accept_connections print buffer */
        connection_array = (struct connection_info *)(socket_blocks_p);

#ifdef POLLSET
        pollset_events = (POLLEV_STRUCT *)(socket_blocks_p + px); /* pollset events */
        if ((pollset_fd = POLLSE_CREATE(pollset_maxevents)) >= 0)
        {
#if (POLLSET & POLLSET_EDGE_TRIG)
          edge_triggered_event = 0;           /*  initialise flag to track occurrence of edge_triggered_events */
#else
          send_recv_oppy_count = 0;  /* count of number of connections that have a deferred opportunity for send or recv */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  */
#endif
        for (connindex = 0; connindex < enquirerproccount; connindex++)
        {
          connection_array[connindex].con_socket_desc = -1;  /*  indicate not connected */
          /******   no need to zero any fields, all memset earlier
          connection_array[connindex].con_numsent = connection_array[connindex].con_numrecv
          = connection_array[connindex].con_nummsgr = connection_array[connindex].con_blk_offset = 0;
          ******/
        }

        for (ix=0; ix<CONN_SOCK_SIZE; ix++) conn_by_socket[ix] = -1;  /*  init cache of connections  */
        connectioncount = sockclosecount = sendcount = recvcount = msgoutcount = msgincount = selectrc = 0;   /*  init counters  */
        responder_addrinfo_index = ~0;

        /* Create socket.  for UDP,  create enquirerproccount sockets here since we dont listen/accept */
	cx = 0;            /*  enquirer index */
#ifdef UDP
	max_socket_desc = 0;
	while (cx < enquirerproccount) {
#endif /*  UDP */

        /*   here,  on first connection only, we may loop over all valid responder sockets returned by getaddrinfo */
        try_specific_responder_socket:
            if (cx == 0) {
              if (responder_addrinfo_index == ~0)
                    responder_addrinfo_index = 0;
              else  {
                    responder_addrinfo_index++;
                    if (responder_addrinfo_index == maxrespondercount) {
                      printline(stderr,"responderfn: responder %d: cannot connect: exhausted all responder IP addresses for %s\n",mypid,respondername);
                      goto finish_responder_worker;
                    }
              }
            }
        memcpy( (void *)&responder_socket, (const void *)(respond_addrP[responder_addrinfo_index].ai_addr), sizeof(responder_socket));   /*  seems to be needed to ensure trailing garbage not misinterpreted */
        RECONCILE_PORTS_GENERAL(&responder_socket);                          /*  and reconcile our copy of port number */
        if ((
#ifndef UDP
		max_socket_desc = 
#endif /* not UDP */
		listensd = socket((int)responder_socket.sockaddr_u.opaque_sockaddr.sa_family, (int)IPSOCKTYP, (int)IPPROTCL)) < 0) {
	  		goto sock_fail;
	}
#if ! defined(UDP)
#if defined(POLLSET)
        if ((rc = POLLSE_ADDF(listensd,POLLIN)) != 0)    /*   add to our pollset  */  {
	  		goto poll_add_fail;
	}
        if (verbose > 1)
                      printline(stderr,"responderfn: responder %d: added listener %d to pollset\n",mypid,listensd);
#else  /* not POLLSET */
                      printline(stderr,"responderfn: responder %d: added listener %d to pollset\n",mypid,listensd);
#endif /* not POLLSET */
#endif /* not UDP  */

        SET_PORTS_GENERAL(&responder_socket , ((unsigned short)(lowestport + ( numforked * enquirerproccount ) + cx))); /* port on which this responder worker should listen */

        printline(((verbose > 0) ? stderr : 0)
			,"%Kresponderfn: responder %d: about to bind listening socket desc %d to family %hu addr %s port %hu addrlen %d\n"
                        ,&responder_socket ,hostBfr1
                        ,mypid,listensd,responder_socket.sockaddr_u.opaque_sockaddr.sa_family
                        ,hostBfr1,ntohs(responder_socket.sin_port),respond_addrP[responder_addrinfo_index].ai_addrlen);
        on = 1 , setsockopt(listensd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
          /*  dear oh dear    difficult to get these syscall parameters correct */
        if (verbose > 2) {
            struct sockaddr_general sdname;
            socklen_t namelen;
            int src;
            namelen = sizeof(struct sockaddr_general);
            printline(stderr,"name information for listensd\n");
            if ((src = getsockname(listensd, ((struct sockaddr *)(&sdname)), &namelen)) == 0) {
              RECONCILE_PORTS_GENERAL(&sdname);                      /*  reconcile our copy of port number */
              print_nameinfo(&sdname, namelen);
            }
            printline(stderr,"name information for respondersock\n");
            print_nameinfo(&responder_socket, respond_addrP[responder_addrinfo_index].ai_addrlen);
        }

        if ((rc = (bind(listensd, (struct sockaddr *)&responder_socket, ((socklen_t)(respond_addrP[responder_addrinfo_index].ai_addrlen))))) < 0)  {

                /*    on first connection only, try next combination of enquirer and responder IP addresses */
                if (cx == 0) {
                  printline(stderr,"responderfn: responder %d: binding socket errno %d %s\n",mypid,errno,strerror(errno));
                  goto try_specific_responder_socket;
                }

 	  	goto bind_fail;
	}
#if (POLLSET & POLLSET_EDGE_TRIG)
                  /* set the socket nonblocking */
                  if (length = 1, ioctl(listensd, FIONBIO, &length ) < 0 )
                  {
                    rc = errno;
                    printline(stderr,"responderfn: responder %d: ioctl FIONBIO on listensd %d failed errno %d %s\n",mypid,listensd,rc,strerror(rc));
                    goto finish_responder_worker;
                  }
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  */
            /********* Find out assigned port number and save it
                       length = sizeof(responder_socket);
                       if (getsockname(sd, (struct sockaddr *)&responder_socket, &length)) {
                       printline(stderr,"netrush responder: getting socket name");
                       return (203);
                       }
                       RECONCILE_PORTS_GENERAL(&responder_socket);
                       printf("Socket has port #%d\n", ntohs(responder.sin_port));
            *********/
#ifdef UDP
		  if ((rc = add_responder_connection(connection_array ,listensd
			    ,conn_by_socket ,&connectioncount ,&max_socket_desc)) < 0) {
		    	goto add_conn_fail;
		  }
		  cx++;
	}  /*  for (cx = 0; cx < enquirerproccount; cx++) */
        listensd = -1;     /*   ensure no accidental action on listensd if UDP */
#endif /*  UDP */

            /* Start accepting connections */
#ifndef UDP
            if ((rc = (listen(listensd, (enquirerproccount+5)))) >= 0)
            {
              /*   seems to be a good idea to set the tcp-level options in the listening socket -
              **   on some OSs the message sockets inherit these.
              */
              if (   (tcp_nodelay)
                  && ((setsockopt(listensd, IPPROTCL, TCP_NODELAY, (char *)&tcp_nodelay, sizeof(tcp_nodelay))) < 0)
                 )
              {
                rc = errno;
                printline(stderr,"responderfn: responder %d: listener socket desc %d TCP_NODELAY failed errno %d %s\n",mypid,listensd,rc,strerror(rc));
              }
                /*  note - linux tcp (man page for tcp) claims to support RFC1323 without requiring anything to be set at tcp level */
#ifdef TCP_RFC1323
              if (   (tcp_rfc1323)
                  && ((setsockopt(listensd, IPPROTCL, TCP_RFC1323, (char *)&tcp_rfc1323, sizeof(tcp_rfc1323))) < 0)
                 )
              {
                rc = errno;
                printline(stderr,"responderfn: responder %d: listener socket desc %d TCP_RFC1323 failed errno %d %s\n",mypid,listensd,rc,strerror(rc));
              }
#endif
#endif /* not UDP */

              /***********************************************************************************************
              **   logic for responder is fairly simple :
              **   select on open sockets including listener
              **   always accept new connection requests on listener
              **     (not after all have been accepted at least once unless compiled with -D AD_NAUSEAM)
              **   always read all available messages from ready sockets
              **   send on socket marked as writable provided we have sent fewer than we have received on this connection
              **   this last rule is so as to avoid flooding enquirer with too many incoming messages
              **   (since he is bound to receive all incoming messages before doing anything else)
              **   if not compiled with -D AD_NAUSEAM, terminate after all connections have been closed -
              **   for UDP,  since we have no "connection"s to monitor,  we base termination on consecutive timeouts.
              **************************************************************************************************/
#ifdef UDP /*  for UDP,  extra logic to check for termination */
	      num_consctv_sel_tmouts = 0; /*  number of consecutive select timeouts - to decide on responder termination */
#endif  /*   for UDP,  extra logic to check for termination */
              do
              {
                /*  SIGXCPU is for any experimenting - process will do something once and then clear it */
                if (signalled == SIGXCPU)
                {
                  /*   try turning on TCP_NODELAY dynamically if it was on, else turn it off  */
                  int temp_sd;
                  /*  we should really maintain a shadow flag to track on or off but that clutters up the non-experimental code ... */
                  tcp_nodelay = 1 - tcp_nodelay;    /* ... so simply toggle the user parm */
                  for (jx = 0; jx < enquirerproccount; jx++)
                  {
                    if ((temp_sd = connection_array[jx].con_socket_desc) >= 0)
                    {
                      if ((setsockopt(temp_sd, IPPROTCL, TCP_NODELAY, (char *)&tcp_nodelay, sizeof(tcp_nodelay))) < 0)
                      {
                        rc = errno;
                        printline(stderr,"responderfn: responder %d: conn index %ld socket desc %d TCP_NODELAY failed errno %d %s\n",mypid,jx,temp_sd,rc,strerror(rc));
                      }
                      else printline(stderr,"responderfn: responder %d: conn index %ld socket desc %d TCP_NODELAY now set to %d\n",mypid,jx,temp_sd,tcp_nodelay);
                    }
                  }
                  signalled = 0;  /*  clear */
                }
              setup_select:
                no_work_done = 1;         /*   no work done as yet */
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
        /*    The following code is tedious.
        **    Find each case where registered events do not match desired events and issue commands to modify or delete/add.
        **    Semantics of MOD : new events replace existing
        */
        for (connindex = 0; connindex < enquirerproccount; connindex++)
        {
          sd = connection_array[connindex].con_socket_desc;
          if (sd >= 0)  /*  connected */
          {

            /*  assume we never unregister for POLLIN - phew! */

#if (XSEND_LIMIT >= 0)  /*   do not consider writes if enquirer has negative send limit  */
            /*  if message out allowed but not registered, then register  */
            if (   (!shutting_down)
                && (connection_array[connindex].con_nummsgw < connection_array[connindex].con_nummsgr)
                && (!(connection_array[connindex].pollset_flags & POLLSET_EV_POLLOUT))   /* not registered  */
               )
            {
              if (!(rc = POLLSE_MODF(sd, ( POLLOUT|((connection_array[connindex].pollset_flags & POLLSET_EV_POLLIN) ? POLLIN : 0) ))))
              {
                connection_array[connindex].pollset_flags |= POLLSET_EV_POLLOUT; /* remember registered POLLOUT */
                if (verbose > 1)
                  printline(stderr,"responder: %d: connindex %d registered POLLOUT on sd %d\n",mypid,connindex,sd);
                chshmp->ch_num_dynreg++;
              }
              else
              {
                rc = errno;
                printline(stderr,"responderfn: responder %d: connindex %d pollctl register POLLOUT on sd %d errno %d %s\n",mypid,connindex,sd,rc,strerror(rc));
                goto finish_responder_worker;
              }
            }
            /*  if message out not allowed but registered, then un-register  */
            else
#endif  /*  (XSEND_LIMIT >= 0)    do not consider writes if enquirer has negative send limit  */
            if (   (connection_array[connindex].pollset_flags & POLLSET_EV_POLLOUT)   /* registered  */
#if (XSEND_LIMIT >= 0)  /*   do not consider writes if enquirer has negative send limit  */
                &&
                   (   (shutting_down)
                    || (connection_array[connindex].con_nummsgw >= connection_array[connindex].con_nummsgr)
                   )
#endif  /*  (XSEND_LIMIT >= 0)    do not consider writes if enquirer has negative send limit  */
               )
            {
#if (POLLSET & POLLSET_DYNAMIC_POLLIN)
              if (!(rc = POLLSE_DEL(sd)))    /*   remove from our pollset  */
              {
                connection_array[connindex].pollset_flags &= ~POLLSET_EV_POLLOUT; /* unregister POLLOUT but preserve other flags temporarily for next test */
                if (verbose > 1)
                  printline(stderr,"responder: %d: connindex %d  deleted socket %d from set\n",mypid,connindex,sd);
                chshmp->ch_num_dynung++;
              }
              else
              {
                rc = errno;
                printline(stderr,"responderfn: responder %d: connindex %d pollctl del sd %d errno %d %s\n",mypid,connindex,sd,rc,strerror(rc));
                goto finish_responder_worker;
              }
              if (   (connection_array[connindex].pollset_flags & POLLSET_EV_POLLIN)  /* if we had been registered for POLLIN */
                  && (rc = POLLSE_ADDF(sd, POLLIN))
                 )
              {
                rc = errno;
                printline(stderr,"responderfn: responder %d: connindex %d pollctl register POLLIN on sd %d errno %d %s\n",mypid,connindex,sd,rc,strerror(rc));
                goto finish_responder_worker;
              }
#else /* linux */
              /*   optimize for linux when no dynamic POLLIN so entry never deleted - use MOD to remove POLLOUT directly */
              if (!(rc = POLLSE_MODF(sd, POLLIN)))
              {
                connection_array[connindex].pollset_flags &= ~POLLSET_EV_POLLOUT; /* unregister POLLOUT but preserve other flags temporarily for next test */
                if (verbose > 1)
                  printline(stderr,"responder: %d: connindex %d  modified socket %d dropped POLLOUT\n",mypid,connindex,sd);
                chshmp->ch_num_dynung++;
              }
              else
              {
                rc = errno;
                printline(stderr,"responderfn: responder %d: connindex %d pollctl modify drop POLLIN on sd %d errno %d %s\n",mypid,connindex,sd,rc,strerror(rc));
                goto finish_responder_worker;
              }
#endif /* linux */
            }
          }

          else if (connection_array[connindex].pollset_flags & (POLLSET_EV_POLLIN|POLLSET_EV_POLLOUT))  /*  registered  */
          {
            if (!(rc = POLLSE_DEL(sd)))    /*   remove from our pollset  */
            {
              connection_array[connindex].pollset_flags &= ~(POLLSET_EV_POLLIN|POLLSET_EV_POLLOUT); /* now no registered events */
                    if (verbose > 1)
                      printline(stderr,"responder: %d: connindex %d  deleted socket %d from set\n",mypid,connindex,sd);
            }
            else
            {
              rc = errno;
              printline(stderr,"responderfn: responder %d: connindex %d pollctl del sd %d errno %d %s\n",mypid,connindex,sd,rc,strerror(rc));
              goto finish_responder_worker;
            }
          }
        }
#elif !defined(POLLSET)
                /*   set up the select lists  */
                Nfdsmsgs = max_socket_desc + 1;
                FD_ZERO(&readlist);
                FD_ZERO(&writelist);
#ifndef UDP
#ifndef AD_NAUSEAM
                /*  don't accept any more after we have accepted all enquirers at least once */
                if ((connectioncount + sockclosecount) < enquirerproccount) /* some enquirers have never been accepted */
#endif
                  FD_SET(listensd, &readlist);
#endif  /* not UDP */
                /***   FD_ZERO(ExceptList);   ***/
                for (connindex = 0; connindex < enquirerproccount; connindex++)
                {
                  sd = connection_array[connindex].con_socket_desc;
                  if (sd >= 0)  /*  connected */
                  {
                    FD_SET(sd, &readlist);
#if (XSEND_LIMIT >= 0)  /*   do not consider writes if enquirer has negative send limit  */
                    /* do not send more than the number of full messages we have received */
                    /* and if we are shutting down, don't look for sockets ready for sending */
                    if (   (!shutting_down)
                        && (connection_array[connindex].con_nummsgw < connection_array[connindex].con_nummsgr)
                       )
                    {
                      FD_SET(sd, &writelist);
                    }
                    /***   FD_SET(connection_array[connindex].con_socket_desc, ExceptList);   ***/
#endif  /*  (XSEND_LIMIT >= 0)    do not consider writes if enquirer has negative send limit  */
                  }
                }
#endif /* not POLLSET */
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  for edge-triggered, avoid the poll_wait if we found an edge-triggered event   */
        if (!edge_triggered_event)
        {
#endif /* for edge-triggered, avoid the poll_wait if we found an edge-triggered event   */
              do_select:
                if (shutting_down)
                  { TimeOut.tv_sec = 2;  TimeOut.tv_usec = 0; }
                else
                  { TimeOut.tv_sec = 0;  TimeOut.tv_usec = 500000; }
                selectrc =
#ifdef POLLSET
                  POLLEV_GET_OR_WAIT(pollset_fd, pollset_events, pollset_maxevents, ((TimeOut.tv_sec*1000)+(TimeOut.tv_usec/1000)));
#else /* select */
                  select(Nfdsmsgs, &readlist, (shutting_down ? 0 : &writelist), /*** &ExceptList ***/ 0, &TimeOut);
#endif /* select  */
                  chshmp->ch_num_selpol++;
#if defined(POLLSET) && (! (POLLSET & POLLSET_EDGE_TRIG) )
                  pollin_oppys_count = 0;    /*   initialise count of pollin opportunities */
#endif /* POLLSET  */
                  if (verbose > 3)
                    printline(((verbose > 2) ? stderr : 0),"responderfn: responder %d: rc %d from "
#ifdef POLLSET
                        "poll"
#else /* select */
                        "select"
#endif /* select  */
			      " shtdn= %d Nfdsmsgs= %d\n",mypid,selectrc,shutting_down,Nfdsmsgs);
                if (selectrc == -1)
                {
                  rc = errno;
                  if (signalled == SIGTERM)
                    shutting_down = 1;   /*  commence graceful termination */
                  if ((rc == EINTR) || (rc == EAGAIN))
                    goto do_select;
                  printline(stderr,"responderfn: responder %d: select errno %d %s\n",mypid,rc,strerror(rc));
                  goto finish_responder_worker;
                }

#ifdef UDP /*  for UDP,  extra logic to check for termination */
                if (      (selectrc == 0)
                     &&   ((++num_consctv_sel_tmouts) > UDP)
                     &&   ((connectioncount + sockclosecount)
#ifndef AD_NAUSEAM
                             ==
#else
                             >=
#endif  /* AD_NAUSEAM */
                                      enquirerproccount) /* all enquirers have been accepted */
                     &&   (numenquirers_received_msg == enquirerproccount)    /* all enquirers have sent a message */
		   ) { /* 5 timeouts/no new input indicates all enquirers have probably terminated */
			  shutting_down = 1;   /*  commence graceful termination */
	                  if (verbose > 1)
		               printline(((verbose > 2) ? stderr : 0),"responderfn: responder %d: starting shutdown after %d consecutive timeouts\n"
			           ,mypid,num_consctv_sel_tmouts);
		}
#endif  /*   for UDP,  extra logic to check for termination */

                /***********************************************************************************************
                ** if select reported any send/recv opportunities, then take them
                ** check read lists first, then write lists
                *************************************************************************************************/
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  for edge-triggered, avoid the poll_wait if we found an edge-triggered event   */
        }   /*  if (!edge_triggered_event)  */
#endif /* for edge-triggered, avoid the poll_wait if we found an edge-triggered event   */

#ifdef UDP /*  for UDP,  extra logic to check for termination */
        	/*    if useful work detected,  then reset our count of consecutive select timeouts  */
	        if (        (selectrc > 0)
#if (POLLSET & POLLSET_EDGE_TRIG)
                       ||   (edge_triggered_event)
#endif /* for edge-triggered   */
		   ) {
		            num_consctv_sel_tmouts = 0; /*  reset number of consecutive select timeouts  */
		   }
#endif  /*   for UDP,  extra logic to check for termination */
	      
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  don't check retcode from poll_wait for edge-triggered */
                if (selectrc > 0)      /*  if we have something from select */
                {
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check all sockets for edge-triggered */
          edge_triggered_event = 0; /*  initialise indicator of whether we found an event */
#ifndef UDP
          /*   we want to check the listensd each time -
          **   to do so, create an artificial connindex of -1 for that case and jump inside loop on connindex
          */
#ifndef AD_NAUSEAM
          if ((connectioncount + sockclosecount) < enquirerproccount) /* some enquirers have never been accepted */
          {
#endif  /* AD_NAUSEAM */
            sd = listensd;  /*   initial sd */
            connindex = -1;
            goto accept_again;
#ifndef AD_NAUSEAM
          }
#endif  /* AD_NAUSEAM */
#endif /* not UDP */
        for (connindex = 0; connindex < enquirerproccount; connindex++)
        {
          sd = connection_array[connindex].con_socket_desc;
          if (sd >= 0)  /*  connected */
          {
#elif defined(POLLSET)
                  for (evx = 0; evx < selectrc; evx++)
                  {
                    if (pollset_events[evx].POLLEV_TYP & POLLIN)   /* ready for reading */
                    {
                      pollin_oppys_count++;    /*   maintain count of pollin opportunities */
                      sd =  pollset_events[evx].POLLEV_FD;
#else /* select    */
                  int num_selected;          /*  number of fds returned from select */
                  num_selected = selectrc;          /*  number of fds returned from select */
                  fdwordp = (long *)&readlist;  /*  start searching for bits here */
                  sd = 0;                       /*  first candidate socket */
                  do
                  {
                    if (jx = *fdwordp)
                    {                           /*  some socket set in this long-word  */
                      for (kx = ((sizeof(long))*8); kx > 0; kx--)
                      {
                        if (jx & 0x01)
                        {
#endif /* select  */
                          if (
#ifdef POLLSET
                              (
#endif /* POLLSET */
                              sd == listensd                 /*  this is the listener */
#ifdef POLLSET
                              )
                              /*   for pollset we must check this condition here since listener sd is always polled */
#ifndef AD_NAUSEAM
                             && ((connectioncount + sockclosecount) < enquirerproccount) /* some enquirers have never been accepted */
#endif  /* AD_NAUSEAM */
#endif /* POLLSET */
                              )
                          {
                          accept_again:
                            if (signalled == SIGTERM)
                                shutting_down = 1;   /*  commence graceful termination */
                            accept_parm.listensd = listensd;
                            accept_parm.connection_array = connection_array;
                            accept_connections((void *)&accept_parm);
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                            if (    (accept_parm.accept_rc >= 0 )
                                 || ((accept_parm.msgsock != EAGAIN) && (accept_parm.msgsock != EWOULDBLOCK) )
                               )
                            {
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                            if ((length = (accept_con_pbuf_cur - accept_con_pbuf_start)) > 0)
                            {
                              printline(((verbose > 0) ? stderr : 0),"%.*s",length,accept_con_pbuf_start);
                              accept_con_pbuf_cur = accept_con_pbuf_start;
                            }
                            if (accept_parm.accept_rc >= 0)
                            {
                              if ((rc = add_responder_connection(connection_array ,accept_parm.msgsock
                                        ,conn_by_socket ,&connectioncount ,&max_socket_desc)) == 0)
                              {
                                no_work_done = 0;         /*   work done */
                              }
                              else
                              {
                                if (rc < 0)               /*  if serious error */
                                  shutting_down = 1;   /*  commence graceful termination */
                              }
                            }
                            else
                            {
                              if (signalled == SIGTERM)
                                shutting_down = 1;   /*  commence graceful termination */
                              rc = accept_parm.msgsock;
                              if (rc == EINTR || rc == EAGAIN)
                                goto accept_again;
                              printline(stderr,"responderfn: responder %d: accept errno %d %s\n",mypid,rc,strerror(rc));
                              shutting_down = 1;   /*  commence graceful termination */
                              goto check_read_selects;
                            }
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                            }
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                          }
                          /*  we must check sd again here in case highest lies inside this byte */
#ifndef POLLSET
                          else if (sd > max_socket_desc)
                            goto done_readlist;
#endif /* select  */
                          else
                          {            /*   non-listener socket ready for read  */
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  connindex already set for edge-triggered */
                            connindex = find_connection(sd, conn_by_socket, enquirerproccount, connection_array); /* locate connection */
                            if (connindex >= 0)
                            {
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
                              /* receive a message on this connection */
                            recv_again:
                              /*  treat input msg len of 0 like CONNRESET */
                              memset(inmsgblok,0,MSG_BLOK_SZ);  /*  avoid misinterpreting content of incoming message */
#ifdef UDP
                              already_received_msg = connection_array[connindex].othrend_socket.sin_port; /* base whether we have received message from this otherend before on port num */
#endif /*  UDP */
                              /*  NOTE  - we include case of rc == 0 in following since that is what is returned if ENOTCONN   */
                              if ((send_recv_len = IPRECV_SD(sd, inmsgblok, MSG_BLOK_SZ, recv_flags ,connindex)) <= 0)
                              {
                                rc = errno;
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
#ifdef DEBUG_NETRUSH
                                printline(((verbose > 2) ? stderr : 0),"responderfn: responder %d: connindex %d sd %d recv error rc %d errno %d\n",mypid,connindex,sd,send_recv_len,rc);
#endif  /* DEBUG_NETRUSH */
                                if  (   (send_recv_len == -1)
                                     && ( (EAGAIN == rc) || (EWOULDBLOCK == rc) )
                                    )
                                  chshmp->ch_num_wdblck++;       /*  number of cases of EAGAIN and EWOULDBLOCK for edge-triggered */
                                else
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                                if (!(rc = handle_recv_error(sd,connindex,conn_by_socket,enquirerproccount,connection_array,0,rc,send_recv_len)))
                                  goto recv_again;
                              }
                              else
                              {
#ifdef MSG_OUTPUT_TO_FILE
                                /*  write received mesg to output file if specified */
                                if (my_msg_output_fd > 0) {
                                    /*  handle here the simple cases of
                                    **    either binary byte-interleaved
                                    **    or     single enquirer connection - interleaving cannot occur
                                    **    or     no pending line_spanned_msgblok and received msg ends in linefeed -
                                    **  handle all other cases in handle_line_spanned_msg
                                    */
                                    if (    (MY_MSG_OUTPUT_BINARY == my_msg_output_type)                 /* binary content   */
                                         || (enquirerproccount == 1)                                          /* only one enquirer  */
                                         || (    (0 == connection_array[connindex].line_spanned_msgblok) /* no pending       */
                                              && ('\n' == *(inmsgblok + send_recv_len - 1))              /* ends in linefeed */
                                            )
                                       ) {
                                        write(my_msg_output_fd , inmsgblok , send_recv_len);
                                    } else {
                                        rc = handle_line_spanned_msg(&(connection_array[connindex]) , my_msg_output_fd , &inmsgmall , &inmsgblok , send_recv_len);
                                        if (rc < 0)
                                            goto finish_responder;
                                    }
                                }
#endif  /* MSG_OUTPUT_TO_FILE */
#ifdef UDP
                                /*  for UDP,  the first message received from an otherend initiates our record of the pseudo-connection */
                                if (already_received_msg == 0) {
				    socket_name_p = &(connection_array[connindex].othrend_socket);
 			            printline(((verbose > 0) ? stderr : 0)
                                               ,"%Kresponderfn: responder %d: recvd first msg on conn index %ld socket desc %d from enquirer addr %s port %hu\n"
                                               ,socket_name_p ,hostBfr1
                                               ,mypid,connindex,sd
                                               ,hostBfr1,ntohs(socket_name_p->sin_port)
                                             );
                     		    numenquirers_received_msg++;  /*  total number of enquirers who have sent us a message  */
				}
#endif /*  UDP */
                                rc = handle_recv_completion(chshmp ,sd,connindex ,conn_by_socket ,enquirerproccount ,connection_array ,0,send_recv_len);
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  remember successful event for edge-triggered */
                                if (!rc)
                                  edge_triggered_event = 1; /*  note we found an event */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  remember successful event for edge-triggered */
                              }
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  connindex already set for edge-triggered */
                            }
                            else
                            {
                                /*  if (verbose > 1)  */
                                printline(stderr,"responderfn: responder %d: can't find connindex for socket %d\n",mypid,sd);
                            }
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
                          }
                        check_read_selects:
#ifndef POLLSET
                          if (--num_selected == 0)  /*  if we've seen all notifying sockets */
                            goto done_select_list;
                        }
                        jx >>= 1;    /*  examine next bit  ...            */
                        sd++;        /*  ... corresponding to this socket */
                      }
                    }
                    else sd += ((sizeof(long))*8);    /*  advance socket number past this empty long-word */
                    fdwordp++;       /*  advance to next select list byte */
                  } while (sd <= max_socket_desc); /*  for each connected socket */
#elif ! (POLLSET & POLLSET_EDGE_TRIG)  /* POLLSET  */
            ;
            }
          }
#else  /* POLLSET_EDGE_TRIG  */
                    ;
                    }
                  }
#endif /* POLLSET_EDGE_TRIG  */
                done_readlist:

#ifdef POLLSET
                  /*   for polling, we first revisit all deferred opportunities as noted in the connection entry,
                  **   then check each event returned by poll
                  */
                  if (!shutting_down)
                  {
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check all sockets for edge-triggered */
                    for (connindex = 0; connindex < enquirerproccount; connindex++)
                    {
                      sd = connection_array[connindex].con_socket_desc;
                      if (   (sd >= 0)  /*  connected */
#if (XSEND_LIMIT >= 0)  /*   do not consider writes if enquirer has negative send limit  */
                          && (connection_array[connindex].con_nummsgw < connection_array[connindex].con_nummsgr)
#endif  /*  (XSEND_LIMIT >= 0)    do not consider writes if enquirer has negative send limit  */
                         )
                      {
#else /* not edge-triggered */
                  long send_recv_oppy_decrt; /* decrement count of number of connections that have a deferred opportunity for send or recv */
                  send_recv_oppy_decrt = send_recv_oppy_count;
                  connindex = -1;   /*   start looking for deferred opportunities after here */
                  for (evx = 0; ((send_recv_oppy_decrt > 0) || (evx < selectrc)); ((send_recv_oppy_decrt > 0) ? send_recv_oppy_decrt-- : evx++))
                  {
                    if (send_recv_oppy_decrt > 0)   /*  reconsider this opportunity */
                    {
                      /*   search forwards for next pollout, starting from next connindex   */
                      do
                      {
                        connindex++;
                        if (connindex >= enquirerproccount)
                        {
                          printline(stderr,"responderfn: responder %d: exhausted sds while searching for %d remaining opportunities\n",mypid,send_recv_oppy_decrt);
                          shutting_down = 1;   /*  commence graceful termination */
                          rc = 778;
                          goto finish_responder_worker;
                        }
                      }
                      while (!(connection_array[connindex].pollset_flags & POLLSET_SEND_OPPY));
                      sd = connection_array[connindex].con_socket_desc;
                      connection_array[connindex].pollset_flags &= ~POLLSET_SEND_OPPY;   /*  switch off to handle  */
                      send_recv_oppy_count--;                   /*   keep count of these */
#ifdef DEBUG_NETRUSH
                      printline(((verbose > 2) ? stderr : 0),"responderfn: responder %d: reconsidering deferred opportunity for sd %d , %d remaining opportunities\n",mypid,sd,send_recv_oppy_decrt);
#endif  /* DEBUG_NETRUSH */
                      goto consider_polled_sd;
                    }
                    if (pollset_events[evx].POLLEV_TYP & POLLOUT)
                    {
                      sd =  pollset_events[evx].POLLEV_FD;
#endif /* not edge-triggered */
#else /* select  */
                  fdwordp = (long *)&writelist;  /*  start searching for bits here */
                  sd = 0;                        /*  first candidate socket */
                  do
                  {
                    if (jx = *fdwordp)
                    {                           /*  some socket set in this long-word  */
                      for (kx = ((sizeof(long))*8); kx > 0; kx--)
                      {
                        if (jx & 0x01)
                        {
                          /*  we must check sd again here in case highest lies inside this byte */
                          if (sd > max_socket_desc)
                            goto done_writelist;
                          else
#endif /* select  */
                          if (sd != listensd)    /*  ignore listener for sends */
                          {
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  connindex already set for edge-triggered */
                            connindex = find_connection(sd, conn_by_socket, enquirerproccount, connection_array); /* locate connection */
                            if (connindex >= 0)
                            {
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG)  connindex already set for edge-triggered */
#if defined(POLLSET) && ( ! (POLLSET & POLLSET_EDGE_TRIG) )  /*  msg counts already checked for edge-triggered */
                              /*   for pollset we must ensure we do not send more than the number of full messages
                              **   we have received, and we must check this condition here since all sds are polled
                              */
                            consider_polled_sd:
                              if (connection_array[connindex].con_nummsgw < connection_array[connindex].con_nummsgr)
                              {
#endif /* POLLSET &&  ! (POLLSET & POLLSET_EDGE_TRIG) msg counts already checked for edge-triggered */
#if (XSEND_LIMIT >= 0)  /*   do not consider writes if enquirer has negative send limit  */
                              /* send a message on this connection */
                              sprintf(outmsgblok+MESSAGE_HDR_SIZE,"%lu",sendcount); /* crude message tracking */
                            send_again:
                              if ((send_recv_len = IPSEND_SD(sd, connindex)) == -1)
                              {
                                rc = errno;
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                                if  ( (EAGAIN == rc) || (EWOULDBLOCK == rc) )
                                  chshmp->ch_num_wdblck++;       /*  number of cases of EAGAIN and EWOULDBLOCK for edge-triggered */
                                else
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  ignore EAGAIN and EWOULDBLOCK for edge-triggered */
                                if (!(rc = handle_send_error(sd,connindex,conn_by_socket,enquirerproccount,connection_array,0,rc)))
                                  goto send_again;
                              }
                              else
                              {
                                rc = handle_send_completion(chshmp ,sd,connindex,conn_by_socket, enquirerproccount, connection_array ,0 ,send_recv_len);
#if (POLLSET & POLLSET_EDGE_TRIG)   /*  check whether event found */
                                if (!rc)
                                  edge_triggered_event = 1; /*  note we found an event */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG)  check whether event found  */
                                no_work_done = 0;         /*   work done */
                              }
#ifdef DEBUG_NETRUSH
                              printline(stderr,"responderfn: responder %d: sent %d bytes on connection %d socket %d sentmsgs %d of %d\n"
                                        ,mypid,send_recv_len,connindex,sd,connection_array[connindex].con_sendcount,sendcount);
#endif
#endif  /*  (XSEND_LIMIT >= 0)    do not consider writes if enquirer has negative send limit  */
#if defined(POLLSET) && ( ! (POLLSET & POLLSET_EDGE_TRIG) )  /*  msg counts already checked for edge-triggered */
                            }
                            else
                            {
                              /*   since poll tells us only once about an opportunity to send and we are declining,
                              **   we must remember this send opportunity for next time
                              */
                              /*  the following test should always be true because of the above oneshot point, but check to be safe */
                              if (!(connection_array[connindex].pollset_flags & POLLSET_SEND_OPPY))
                              {
                                connection_array[connindex].pollset_flags |= POLLSET_SEND_OPPY;
                                send_recv_oppy_count++;                   /*   keep count of these */
#ifdef DEBUG_NETRUSH
                                printline(((verbose > 2) ? stderr : 0),"responderfn: responder %d: deferred opportunity for sd %d , %d remaining opportunities\n",mypid,sd,send_recv_oppy_count);
#endif  /* DEBUG_NETRUSH */
                              }
                            }
#endif /* POLLSET &&  ! (POLLSET & POLLSET_EDGE_TRIG) msg counts already checked for edge-triggered */
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  connindex already set for edge-triggered */
                            }
                            else
                            {
                                /*  if (verbose > 1)  */
                                printline(stderr,"responderfn: responder %d: can't find connindex for socket %d\n",mypid,sd);
                            }
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG)  connindex already set for edge-triggered */
                          } /* sd != listensd) ignore listener for sends */
                        check_write_selects:
#ifndef POLLSET
                          if (--num_selected == 0)  /*  if we've seen all notifying sockets */
                            goto done_select_list;
                        }
                        jx >>= 1;    /*  examine next bit  ...            */
                        sd++;        /*  ... corresponding to this socket */
                      }
                    }
                    else sd += ((sizeof(long))*8);    /*  advance socket number past this empty long-word */
                    fdwordp++;       /*  advance to next select list byte */
                  } while (sd <= max_socket_desc); /*  for each connected socket */
#else  /* POLLSET  */
#if ( ! (POLLSET & POLLSET_EDGE_TRIG) )  /*  not edge-triggered */
                    ;
                  }
                  }
#else /*  ( POLLSET & POLLSET_EDGE_TRIG )  check all sockets for edge-triggered */
                    ;
                    }
                    }
#endif /*  ( POLLSET & POLLSET_EDGE_TRIG )  check all sockets for edge-triggered */
                  }  /*   (!shutting_down)   */
#endif /* POLLSET  */
                done_writelist:
                done_select_list: ;
#if ! (POLLSET & POLLSET_EDGE_TRIG)   /*  don't check retcode from poll_wait for edge-triggered */
                }        /*  end we have something from select */
#endif /*  ! (POLLSET & POLLSET_EDGE_TRIG) */
                /*******************************************************************************************************
                **  logic for termination of responder here
                **  check for shutting down - if so, terminate only if timeout on select, to allow readlist to drain
                *******************************************************************************************************/
              } while (
#if   (POLLSET & POLLSET_EDGE_TRIG)   /*  check events for edge-triggered */
                       (edge_triggered_event) /*  an event */
#elif defined(POLLSET)
                       (pollin_oppys_count > 0)    /*   found some pollin opportunities */
#else
                       (selectrc)             /* continue if we got some work  ...   */
#endif
                       || (!shutting_down)    /* ... or are not shutting down        */
                      );
              rc = 0;        /*  no error if we reach here */
              printline(((verbose > 0) ? stderr : 0),"responderfn: responder %d: terminating normally, shutting_down= %d "
                       "selectrc"
                        "= %d\n",mypid,shutting_down,
                       selectrc
                        );
              goto finish_responder_worker;
#ifdef UDP
	    add_conn_fail:
          {
            rc = errno;
            printline(stderr,"responderfn: responder %d: error %d trying to add UDP connection for %d %s\n",mypid,rc,cx,strerror(rc));
            goto finish_responder_worker;
          }
#else /* not UDP */
            }
            else
            {
              rc = errno;
              printline(stderr,"responderfn: responder %d: listen errno %d %s\n",mypid,rc,strerror(rc));
              goto finish_responder_worker;
            }
#endif  /*  not UDP */

	bind_fail:
          {
            rc = errno;
            printline(stderr,"responderfn: responder %d: binding socket errno %d %s\n",mypid,rc,strerror(rc));
            goto finish_responder_worker;
          }
#ifdef POLLSET

	poll_add_fail:
          {
            rc = errno;
            printline(stderr,"responderfn: responder %d: pollctl add listensd %d errno %d %s\n",mypid,listensd,rc,strerror(rc));
            goto finish_responder_worker;
          }
#endif /* POLLSET */

	sock_fail:
        {
          rc = errno;
          printline(stderr,"responderfn: responder %d: create socket errno %d %s\n",mypid,rc,strerror(rc));
          goto finish_responder_worker;
        }
#ifdef POLLSET
      }
      else
      {
        rc = errno;
        printline(stderr,"responderfn: responder %d: couldn't create pollset size %d errno %d %s\n",mypid,pollset_maxevents,rc,strerror(rc));
        goto finish_responder_worker;
      }
#endif  /*  POLLSET */
      }
      else
      {
        printline(stderr,"responderfn: responder %d: couldn't obtain storage size %d for socket structs\n",mypid,ix);
        rc = 111;
        goto finish_responder;
      }
    }
    else if (rc == -1)
    {
      rc = errno;
      printline(stderr,"responderfn: responder %d: fork failed errno %d %s\n",mypid,rc,strerror(rc));
    }
    else        /*   non-zero retcd means I am supervisor and retcd is worker's pid */
    {
      rc = 0;       /*  if supervisor reaches here, set good rc  */
      pidarray[pidindex++] = otherpid;    /*   store pid of forked worker   */
      if (signalled == SIGTERM) {
          goto finish_responder;
      }
    }
  }               /*  end loop on forking workers   */

  /*  both supervisor and worker drop through to here */
 finish_responder:
    printing_ctl |= PRINT_ON;

  return rc;

 finish_responder_worker:
  if (listensd >= 0)  /*  if listening */
    close(listensd);  /*  stop listening */
    printing_ctl |= PRINT_ON;


    get_finish_time(RUSAGE_SELF, &start_time, &finish_time, &accum_time_base);
    printline(((verbose > 2) ? stderr : 0),"responderfn: responder %d : sent %lu recv %lu msgw %lu msgr %lu "
               ": elapsed time %d.%" TVFORMAT "d end time %24.24s %06.6d\n user CPU %d.%06.6d, system CPU %d.%06.6d,\n vol contxt swchs %d, invol contxt swchs %d\n"
              ,mypid,sendcount,recvcount,msgoutcount,msgincount
              ,accum_time_base.tb_high, accum_time_base.tb_low
              ,ctime((time_t *)&(curtime.tv_sec)), curtime.tv_usec
              ,RUsage.ru_utime.tv_sec, RUsage.ru_utime.tv_usec
              ,RUsage.ru_stime.tv_sec, RUsage.ru_stime.tv_usec
              ,RUsage.ru_nvcsw, RUsage.ru_nivcsw
            );

  if (reportFILE)
    fclose (reportFILE);

  /*   at this point, supervisor can proceed to report results without needing to wait for me */
  chshmp->ch_flags |= CH_CHILD_FINISHED;


#ifdef SLEEP_NETRUSH_AT_END
#if (SLEEP_NETRUSH_AT_END == 1)
#undef SLEEP_NETRUSH_AT_END
#define SLEEP_NETRUSH_AT_END 300
#endif
  sleep(SLEEP_NETRUSH_AT_END);
#endif


 responder_shutdown_socks:
#ifdef CLOSONOSOCK
    /*  actually seems safer to leave sockets open ... */
#else
  for (connindex = 0; connindex < enquirerproccount; connindex++)
  {
     if ((sd = connection_array[connindex].con_socket_desc) >= 0)  /*  if connected */
       shutdown(sd,SHUT_RDWR);
  }
#endif
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
  if (socket_blocks_all)   free(socket_blocks_all);
#else
  if (socket_blocks_p)   free(socket_blocks_p);
#endif                        /*  page-aligned message buffers */
#ifdef DYNAMIC_MSG_BUF        /*  malloc'd message buffers */
#if (DYNAMIC_MSG_BUF == 4096) /*  page-aligned message buffers  */
      if (inmsgmall)  free(inmsgmall);  /*   inbound message buffer  */
      if (outmsgmall) free(outmsgmall); /*  outbound message buffer  */
#else                         /*  not page-aligned message buffers */
      if (inmsgblok)  free(inmsgblok);  /*   inbound message buffer  */
      if (outmsgblok) free(outmsgblok); /*  outbound message buffer  */
#endif                        /*  not page-aligned message buffers */
#endif                        /*  malloc'd message buffers */

#ifndef CLOSOPHOBIA    /*  actually seems safer to leave the darned thing open ... */
#endif
#ifdef POLLSET
        close(pollset_fd);
#endif  /*  POLLSET */
 /*  exit here seems to result in lost output so return to main which will return again to OS
     exit (rc);
  */
 return rc;
}

int get_enquirer_responder_names(int option)
/*   get enquirer and responder name pointers and return length of optarg or 0 on error */
{
  int rc;
  char *charptr;

  if (rc = strlen(optarg))
  {
    if (enquirername = malloc(rc+1))
    {
#ifdef DEBUG_NETRUSH
      printline(stderr,"get_enquirer_responder_names: acquired enquirername %p\n",enquirername);
#endif
      strcpy(enquirername,optarg);
      if (    (    (respondername = strchr(enquirername, ','))   /*  assignment intentional */
                && (must_use_ipv6 = 1) /* user insists on using IPV6  assignment intentional */
              )
           || (respondername = strchr(enquirername, ':'))        /*  assignment intentional */
	 ) {
        enquirer_responder_separator = *respondername;  /*  either : or ,  -  , requests use of IPV6 */
        *respondername++ = '\0';  /*  null-terminate the enquirer name */
      }
      else
      {
        printline(stderr,"get_enquirer_responder_names: enquirer:respondername string does not contain : or , separator %s\n",optarg);
        rc = 0;
      }
    }
    else
    {
      printline(stderr,"get_enquirer_responder_names: unable to get storage to hold enquirer:respondername %s\n",optarg);
      rc = 0;
    }
  }
  else printline(stderr,"get_enquirer_responder_names: must specify enquirername:respondername with -%c flag\n",option);
  return rc;
}

int retrieve_addrinfo (char *ip_name_p, struct addrinfo **addrinf_pp, unsigned int *num_addrs, unsigned int scope_id)
/* Retrieve and optionally display the ip address info for the specified ip name
** based on source  http://tldp.org/HOWTO/Linux+IPv6-HOWTO/chapter-section-using-api.html
*/
{

  /*     for addrinfo - IPV6 enablement */
  int              aiErr;
  struct addrinfo *aiHead;
  struct addrinfo *temp_addrinf_p;
  struct addrinfo *targ_addrinf_p;    /*   target arry of addrinfs     */
  struct sockaddr_general *pSadrGen;  /*   target arry of socketaddrs  */
  struct addrinfo  hints;
  struct sockaddr_in   *pSadrIn;
  struct sockaddr_in6  *pSadrIn6;
  unsigned int naddrs, ix;

    /*
    ** Initialize the 'hints' structure for getaddrinfo  -
    ** Set ai_family field to AF_INET6 if user insists on IPv6, otherwise AF_UNSPEC.
    */
    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family   = (must_use_ipv6 ? AF_INET6 : AF_UNSPEC); /* IPv6 or IPv4 */
    hints.ai_socktype = IPSOCKTYP;     /* stream or datagram.                */
    hints.ai_protocol = IPPROTCL;      /* TCP or UDP transport layer.        */

    if ( ( aiErr = getaddrinfo( ip_name_p, 0, &hints, &aiHead ) ) == 0 ) {
      if (addrinf_pp) { /*  wants to copy the address info list */
        /*
        ** copy each item in the list into local arrays,  one array for addrinfos and one for sockaddrs.
        ** We must copy them because the system calls reuse storage and thus overwrite previous.
        */
        for ( naddrs = 0, temp_addrinf_p = aiHead; temp_addrinf_p != NULL; temp_addrinf_p = temp_addrinf_p->ai_next )
        {
            naddrs++;    /*  count number */
        }
        if (    (naddrs)
                /*  store the sockaddrs above the addrinfos aligned on 8-byte boundary */
             && (targ_addrinf_p = malloc(((((naddrs * (sizeof(struct addrinfo))) + 7) >>3) <<3) + (naddrs * sizeof(struct sockaddr_general))))
           ) {
          *addrinf_pp = targ_addrinf_p;
          pSadrGen = ((struct sockaddr_general *)(((((unsigned long)(targ_addrinf_p+naddrs)) + 7) >>3) <<3));
          if (verbose > 2)
            printline(stderr,"%s :naddrs= %d addrinf_p= %p stored in %p\n",ip_name_p,naddrs,(char *)targ_addrinf_p,(char *)addrinf_pp);
          for (ix=0, temp_addrinf_p = aiHead; temp_addrinf_p != NULL;ix++, temp_addrinf_p = temp_addrinf_p->ai_next )
          {
            memcpy((void *)(targ_addrinf_p+ix),(void *)temp_addrinf_p,sizeof(struct addrinfo));  /*  copy the addrinfo */
            memcpy((void *)(pSadrGen+ix),(void *)(temp_addrinf_p->ai_addr),sizeof(struct sockaddr_general)); /* copy the sockaddr */
            RECONCILE_PORTS_GENERAL(pSadrGen+ix);                                       /*  and reconcile our copy of port number */
            targ_addrinf_p[ix].ai_addr = (struct sockaddr *)(pSadrGen+ix);        /*  set addr of copied sockaddr in the addrinfo */
            if (verbose > 2)
              printline(stderr,"%s :index= %d copied to addrinf_p= %p sockaddr at %p\n" ,ip_name_p,ix,(char *)(targ_addrinf_p+ix),(char *)targ_addrinf_p[ix].ai_addr);
            /* if IPv6, ensure the scope ID is set       */
            if (targ_addrinf_p[ix].ai_family == PF_INET6)
            {
               pSadrIn6 = (struct sockaddr_in6 *) targ_addrinf_p[ix].ai_addr;
               if ( pSadrIn6->sin6_scope_id == 0 )  /* IF the scope ID wasn't set. */
               {
                 if ( (scope_id != 0) && (scope_id != UNKNOWN_SCOPE_ID) ) { /* if caller specified scope */
                   pSadrIn6->sin6_scope_id = scope_id;
                   if (verbose > 2)
                     printline(stderr,"%s : set scopeid to %d from -I %s\n" ,ip_name_p,scope_id,local_net_intrfc);
                 }
                 else printline(stderr,"%s : scope was not specified as IPaddress suffix or in -I flag  -  expect failure\n" ,ip_name_p);
               }  /* End IF the scope ID wasn't set. */
            }  /* End IPv6 */
            if (ix)
              targ_addrinf_p[ix-1].ai_next = &targ_addrinf_p[ix];
            if ( verbose ) {
              /*
              ** Display the address information just fetched.
              ** First, common (protocol-independent) info.
              */
              printline( stderr
                      ,"Address info for %s:\n"
                       "   ai_flags     = 0x%02X\n"
                       "   ai_family    = %d (PF_INET = %d, PF_INET6 = %d)\n"
                       "   ai_socktype  = %d (SOCK_STREAM = %d, SOCK_DGRAM = %d)\n"
                       "   ai_protocol  = %d (IPPROTO_TCP = %d, IPPROTO_UDP = %d)\n"
                       "   ai_addrlen   = %d (sockaddr_in = %d, sockaddr_in6 = %d)\n"
                      ,ip_name_p
                      ,targ_addrinf_p->ai_flags
                      ,targ_addrinf_p->ai_family
                      ,PF_INET
                      ,PF_INET6
                      ,targ_addrinf_p->ai_socktype
                      ,SOCK_STREAM
                      ,SOCK_DGRAM
                      ,targ_addrinf_p->ai_protocol
                      ,IPPROTO_TCP
                      ,IPPROTO_UDP
                      ,targ_addrinf_p->ai_addrlen
                      ,sizeof( struct sockaddr_in )
                      ,sizeof( struct sockaddr_in6 ) );
              /*
              ** Display the protocol-specific formatted address.
              */
              if (print_nameinfo((struct sockaddr_general *)targ_addrinf_p->ai_addr , targ_addrinf_p->ai_addrlen))
                goto finish_retrieve;
            }
          }
          *num_addrs = ix;  /*  return count of valid addresses  */
        } else {
          printline( stderr ,"either getaddrinfo for %s failed to return any addresses or malloc of area to copy them failed\n");
          aiErr = -9;
        }
        freeaddrinfo( aiHead );
      }

    }  /* End getaddrinfo successful */ else {
       printline( stderr ,"getaddrinfo failed for %s: ERROR - %s.\n"
               ,ip_name_p
	       ,gai_strerror( aiErr ) );
    }
 finish_retrieve:
    return aiErr;
}

void print_version(void) {
      printline(stderr, "%s version %.1f  protocol " IPPROT_STRING "  event_prioritization" EVENT_PRIORITIZATION_LEGEND POLLSET_DYN_LEGEND POLLSET_TRIG_LEGEND "  XSEND_LIMIT %d\n",command_nameP,NETRUSH_VERSION,XSEND_LIMIT);
      return;
}

int main(int argc, char **argv)
{
  int  rc, clhosterr, svhosterr;
  int sd, length;
  int wantshelp = 0;
  int wantsversion = 0; /*  if set to 1, report version */
  long longrc;
  int ix;
  long nworkers;
  int option;  /*  returned by getopt */
  int remg_parms;   /*  count of args less options */
  pid_t supervisor_pid;
  pid_t pid;
  struct shmid_ds shmid_ds_buff;  /*    required to call shmctl  */
  key_t  shmkey;                  /*    key for shm IPC  */
  size_t shmsize;                 /*    size of shm IPC  */
  int    shmid;                   /*    id of shm IPC    */
  int    job_identifier;    /*   job index - used as proj_id parm of ftok  -  allows several concurrent jobs on same machine */

  int waitrc, waiterrno;
/*  old IPV4-specific
  struct hostent *enquirer_host_entry_p;
  struct hostent *responder_host_entry_p;
*/
  char my_reportdir[256];
  char *reportname;
  struct supvr_workr_common *chshmp;      /*  pointer to worker shared-mem area */
  unsigned long all_numsent;        /*  total number of send calls by all workers  */
  unsigned long all_numrecv;        /*  total number of recv calls by all workers */
  unsigned long all_nummsgw;        /*  total number of messages written by all workers */
  unsigned long all_nummsgr;        /*  total number of messages recvd by all workers */
  unsigned long all_xsend_hwm;      /*  excess of (msgw - msgr) high-water-mark for enquirer workers */
  unsigned long all_numselpol;      /*  number of calls to select or poll_wait */
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
  unsigned long all_numdynreg;       /*  number of calls to dynamically register event */
  unsigned long all_numdynung;       /*  number of calls to dynamically unregister event */
#endif /* POLLSET */
#if (POLLSET & POLLSET_EDGE_TRIG)    /*  count EAGAIN and EWOULDBLOCK for edge-triggered */
  unsigned long all_numwdblck;       /*  number of cases of EAGAIN and EWOULDBLOCK for edge-triggered */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG) count EAGAIN and EWOULDBLOCK for edge-triggered */
  /*  adjusted totals represent the number if all send/recvs had been of full 4k size  */
  double        adj_numsent;        /*  adjusted number of send calls by all workers  */
  double        adj_numrecv;        /*  adjusted number of recv calls by all workers */
  double        all_avg_send_len;   /*  average length of bytes sent per send */
  double        all_avg_recv_len;   /*  average length of bytes rcvd per recv */

/* interval statistics */
timebasestruct_t intvl_start_time, intvl_finish_time; /*  interval start and finish times  */
timebasestruct_t intvl_accum_time_base; /* accumulated delta time_base */
/*  the following are the values till the start of the next interval  */
unsigned long int_till_numsent;        /*  total number of send calls by all workers  */
unsigned long int_till_numrecv;        /*  total number of recv calls by all workers */
unsigned long int_till_nummsgw;        /*  total number of messages written by all workers */
unsigned long int_till_nummsgr;        /*  total number of messages recvd by all workers */
/*  the following are the values at the start of the current interval  */
unsigned long intvl_numsent;        /*  total number of send calls by all workers  */
unsigned long intvl_numrecv;        /*  total number of recv calls by all workers */
unsigned long intvl_nummsgw;        /*  total number of messages written by all workers */
unsigned long intvl_nummsgr;        /*  total number of messages recvd by all workers */

  int copy_of_signalled;            /*  copy of signal received  - to allow resetting the global indicator rapidly  */
  char *charptr;
  char *strend;
  unsigned int nintervals = 1;      /*  number of automatic reporting intervals  */
  unsigned int nfyintvlnum = 0;     /*  notify interval number  */
  char *nfy_host_name = 0;          /*  notify interval ip name */
  struct addrinfo *nfy_addrinf_p;   /*  notify interval ip host addr info */
  unsigned int num_nfy_addrs;
/*  old IPV4-specific
  struct hostent *nfy_host_entry_p; **  notify interval ip hostent **
*/
  struct sockaddr_general nfysockad;     /*  notify ip socket addr */
  unsigned short nfyintvlport = 0;  /*  notify interval port    */
  int nfyrc, nfyerrno;

  /*     for addrinfo - IPV6 enablement */
  int              enquirer_aiErr, responder_aiErr;

#define MAX_NINTERVALS 20
#define NOTIFYLEN 128

  /*  set defaults  */
  rc = 0;
  iamenquirer = -1;
  enquirername = 0;
  respondername = 0;
  nfy_addrinf_p = (struct addrinfo *)0;
  reportFILE = (FILE*)0;  /*  note cannot write to it any more */
  for (ix=0; ix < sizeof(jobname); ix++)
    *(jobname+ix) = '\0';
  time_limit.it_value.tv_sec  = -1;
  max_sendcount = -1;
  verbose = 0;
  mimimal_out = 0;
  job_identifier = 'N';

  /*  set name of command which invoked this program
  **  search back from end for either rightmost / or failing that the start
  */
  charptr = argv[0];  /*  start  */
  ix = strlen(charptr);
  if (ix > 0) {  /*  you never know ... */
      command_nameP = charptr + ix; /*  beyond end */
      charptr = command_nameP - 1;  /*  last char  */
      /*  the following loop iterates at most ix times,
       *  at the end of which (if it did not terminate earlier)
       *  command_nameP -> start and charptr -> char before start
       *  but *charptr is accessed for test only if ix > 0
      */
      do {
          charptr--;
          command_nameP--;
          ix--;
      } while ( (ix > 0) && (*charptr != '/') );
  }
  else command_nameP = netrush_string;

  while ( (option = getopt (argc, argv, ":c:d:e:k:l:m:n:o:O:p:r:s:C:S:I:NRWVqthvwi")) != -1)
  {
#ifdef DEBUG_OPTION
    printline(stderr,"option %c\n",option);
#endif
    switch (option)
    {
    case 'c':
      if (iamenquirer == -1)
      {
        if (!(iamenquirer = get_enquirer_responder_names(option)))
          return 3;
        iamenquirer = 1;
      }
      else iamenquirer = -2;
      break;
    case 's':
      if (iamenquirer == -1)
      {
        if (!(rc = get_enquirer_responder_names(option))) {
          rc = 5;
        } else {
          rc = 0;
        }
        iamenquirer = 0;
      }
      else iamenquirer = -2;
      break;
    case 'I':
      if (strlen(optarg) > (sizeof(local_net_intrfc)-1))
      {
         fprintf( stderr, "%s: network interface name too long %s\n",command_nameP, optarg );
         wantshelp == 1;
      }
      strncpy(local_net_intrfc,optarg,(sizeof(local_net_intrfc)-1));
      local_net_scope_id = if_nametoindex( optarg );
      if ( local_net_scope_id == 0 )
      {
         fprintf( stderr, "%s: Unknown network interface %s\n",command_nameP, optarg );
         wantshelp == 1;
      }
      must_use_ipv6 = 1; /*  user insists on using IPV6    */
      break;
    case 'C':
      enquirerproccount = strtol(optarg,0,10);
      break;
    case 'S':
      responderproccount = strtol(optarg,0,10);
      break;
    case 'd':
      strncpy(jobname, optarg, (sizeof(jobname) - 1));
      break;
    case 'p':
      lowestport = strtoul(optarg,0,10);
      break;
    case 'm':
      max_sendcount = strtol(optarg,0,10);
      break;
    case 'n':
      sosndbufsz = atoi(optarg);
      break;
    case 'e':
      sorcvbufsz = atoi(optarg);
      break;
    case 'l':
      interval_duration.it_value.tv_sec =     /*  interval duration   */
      time_limit.it_value.tv_sec = strtol(optarg, &charptr, 10);
      if (*charptr == ',') {
	nintervals = strtol((charptr+1), &charptr, 10);
	if (nintervals > MAX_NINTERVALS) {
           fprintf(stderr,"nintervals %u too many, limit is %d\n",nintervals, MAX_NINTERVALS);
	   wantshelp == 1;
	}
        else
	if (nintervals < 2) {
           fprintf(stderr,"nintervals %u too few, must be at least 2\n",nintervals);
	   wantshelp == 1;
	}
	interval_duration.it_value.tv_sec /= nintervals;
	interval_duration.it_value.tv_usec = ( (time_limit.it_value.tv_sec % nintervals) * PERSEC ) / nintervals;
	if (*charptr == ',') {
	  nfyintvlnum = strtol((charptr+1), &charptr, 10);
          if (      (*(charptr++) == ',') && (strend = strchr(charptr, ','))
               &&   ((ix = (strend-charptr)) > 0)
               &&   (nfy_host_name = malloc(ix+1))
               &&   (memcpy(nfy_host_name,charptr,ix))
               &&   ((*(nfy_host_name+ix) = '\0') , (0 == retrieve_addrinfo(nfy_host_name, &nfy_addrinf_p, &num_nfy_addrs, local_net_scope_id)))
               &&   (1 <= num_nfy_addrs)
	       &&   nfy_addrinf_p
             ) {
	      /*  phew  -  we have the notify host ip in *nfy_addrinf_p  */
	      memcpy( (void *)&nfysockad, ((char *)(nfy_addrinf_p->ai_addr)), sizeof(nfysockad)); /* take the first one -- not worth the extra effort to try all */
	      SET_PORTS_GENERAL(&nfysockad , (unsigned short)(atoi(strend+1))); /* port on which to send notification */
	  } else {
	      nfyintvlnum = 0; /*  if error,  no notifying */
	  }
	}
      }
      break;
    case 'i':
      printing_ctl = (PRINT_ON | PRINT_TIMESTAMP);
      break;
    case 'k':
      job_identifier = atoi(optarg);
      break;
    case 'o':  /*  binary output file for messages received  */
    case 'O':  /*  linefeed-delimited output file for messages received  */
#ifdef MSG_OUTPUT_TO_FILE
      my_msg_output_type = option;
      ix = (sizeof(msg_output_path)-16);    /*  max size permitted  */
      if (strlen(optarg) > ix)
           fprintf(stderr,"msg_output_path %s too long, limit is %d\n",optarg, ix);
      else strncpy(msg_output_path, optarg, ix);
#else
      fprintf(stderr,"message output to file is not built into this executable\n");
      rc = 81;
#endif  /* not MSG_OUTPUT_TO_FILE */
      break;
    case 'r':  /*  report dir */
      ix = (sizeof(reportdir)-16);    /*  max size permitted  */
      if (strlen(optarg) > ix)
           fprintf(stderr,"report dir %s too long, limit is %d\n",optarg, ix);
      else strncpy(reportdir, optarg, ix);
      break;
    case 't':
      tabular = 1;
      break;
    case 'h':
      wantshelp = 1; /*   wants help  */
      break;
    case 'q':
      mimimal_out = 1;
      break;
    case 'v':
      verbose++;
      break;
    case 'w':
      printing_ctl = 0;
      verbose = 0;
      break;
    case 'R':
      tcp_rfc1323 = 1;
      break;
    case 'N':
      tcp_nodelay = 1;
      break;
    case 'V':
      wantsversion = 1;
      break;
    case 'W':
      msg_waitall = MSG_WAITALL;
      break;

    case '?':
      rc = -1;
      break;

    default:
      printline(stderr, "%s: ERROR: getopt returned character code 0%o for option '%c' unknown.\n",command_nameP
               ,option ,*(argv[optind] + 1));
      rc = -1;
    }
  }

  /*  architectural limit of 32767 processes imposed by using short for conn_by_socket cache
  **  if you need more, change it to int
  */
  if (    (rc == 0)           /*  no errors detected so far     */
       && (argc > 2)          /*  input at least two parms      */
       && (wantshelp == 0)    /*  and didn't request help       */
       && ((remg_parms = (argc-optind)) == 0) /*  didn't input any unrecognized parms  */
       && (  (iamenquirer >= 0) ? 1 :
             (fprintf(stderr,"%s: must specify one and only one of -c, -s\n",command_nameP) , 0)
          )
       && (  (  (enquirerproccount >= 0)   && (enquirerproccount <= 32767)  ) ? 1 :
             (fprintf(stderr,"%s: -C enquirerproccount must be in range 0 - 32767\n",command_nameP) , 0)
	  )
       && (  (  (responderproccount >= 0)   && (responderproccount <= 32767)  ) ? 1 :
             (fprintf(stderr,"%s: -S responderproccount must be in range 0 - 32767\n",command_nameP) , 0)
          )
       && (  (    (  (enquirerproccount > 0)    && (responderproccount > 0)  )
               || (  (enquirerproccount == 0)   && (responderproccount == 0)  )
             ) ? 1 :
             (fprintf(stderr,"%s: if either of enquirerproccount or responderproccount is 0, then they must both be zero\n",command_nameP) , 0)
          )
       && (  (  (lowestport > 0) && ((lowestport + (((2*enquirerproccount)+1) * responderproccount)) <= 65535)  ) ? 1 : /* ensure legal port number */
             (fprintf(stderr,"%s: -p lowestport must be in range 1 - 65535\n"
                             "             and (lowestport + (((2*numenquirers)+1) * numresponders)) must be no greater than 65535\n",command_nameP) , 0)
          )
       && (  (  (enquirername != 0) && (respondername != 0)     ) ? 1 :
             (fprintf(stderr,"%s: must specify both enquirer IP name and responder IP name as enquirer:responder\n",command_nameP) , 0)
          )
       && (  (  (max_sendcount > 0) || (time_limit.it_value.tv_sec > 0)     ) ? 1 :
             (fprintf(stderr,"%s: must specify one or both of -m max_sendcount and -l time_limit\n",command_nameP) , 0)
          )
       && (  (   ((unsigned int)job_identifier & 0x000000ff) && (!((unsigned int)job_identifier & 0xffffff00))   ) ? 1 :
             (fprintf(stderr,"%s: -k job index must be an integer in the range 1-255\n",command_nameP) , 0)
          )
       && (  (       (must_use_ipv6 && (offsetof(struct sockaddr , sa_family) == offsetof(struct sockaddr_in6 , sin6_family)))
                  || ((!must_use_ipv6) && (offsetof(struct sockaddr , sa_family) == offsetof(struct sockaddr_in , sin_family)))
             ) ? 1 :
             (fprintf(stderr,"%s: mismatching offsets of sa_family in struct sockaddr %d and struct sockaddr_inx %d -\nneed re-design!\n",command_nameP
              ,offsetof(struct sockaddr , sa_family) ,(must_use_ipv6 ?  offsetof(struct sockaddr_in6 , sin6_family) : offsetof(struct sockaddr_in , sin_family))) , 0)
          )
     )
  {
    if (wantsversion)
      print_version();
    if (iamenquirer)
    {
      nworkers = enquirerproccount;
      reportname = "/clnt";
    }
    else
    {
      nworkers = responderproccount;
      reportname = "/srvr";
    }
    /*   if user specifies run duration by time_limit instead of max_sendcount,
    **   this is our estimate of how many messages we expect, for our heuristics
    */
    if (max_sendcount > 0)
    {
      expected_sendcount = max_sendcount;
    }
    else
    {
      expected_sendcount = (time_limit.it_value.tv_sec * 40000); /*  asssume 40K messages per sec  -  doesn't have to be exact */
    }

    supervisor_pid = mypid = getpid();
    if (reportdir[0])
    {
      strcpy (my_reportdir, reportdir);
      strcat(my_reportdir, reportname);
      if (jobname[0])
	strcat(my_reportdir, jobname);
      sprintf(my_reportdir + strlen(my_reportdir),"0%d", mypid); /* 0 prefix denotes supervisor */
      reportFILE = fopen(my_reportdir, "w");
    }

    /*  print all commandline args to reportfile if defined and to stderr if verbose */
    if (verbose || reportdir[0])
    {
      for (ix = 0; ix < argc; ix++)
        printline((verbose ? stderr : 0),((ix == (argc-1)) ? "%s\n" : "%s "),argv[ix]);

        if (nfyintvlnum) {              
	      printline(stderr
			    ,"%K%s: notify interval %d to %s port %hu\n"
			    ,&nfysockad ,hostBfr1
			    ,command_nameP,nfyintvlnum
			    ,hostBfr1,ntohs(nfysockad.sin_port));
	}
    }

    recv_flags = msg_waitall;    /*  the only flag which we set at present */

    /* get host details about enquirer IP name and responder IP name
    ** and copy them into our own storage as needed in case overwritten by each syscall
    ** 
    ** Note that :
    **      enquirer needs to know both enquirer and responder IP addresses and uses both while connecting
    **      responder needs to know only its own responder IP address but checks that enquirer IP is resolvable
    **      and,  strangely (and unfriendlily), IPV6 requires scopeid on *all* addresses,  both remote and local
    */
    maxenquirercount = maxrespondercount = 0;
    enquire_addrP = respond_addrP = (struct addrinfo *)0;

    enquirer_aiErr = retrieve_addrinfo( enquirername, ( (iamenquirer) ? &enquire_addrP : (struct addrinfo **)0 ), &maxenquirercount, local_net_scope_id);
    if ((verbose > 3) && maxenquirercount) {
        hostBfr1[0] = '\0';
        printline(stderr,"%Knum_enquirer_addrs= %d first at %p is %s\n"
                  ,enquire_addrP->ai_addr ,hostBfr1,maxenquirercount,(char *)enquire_addrP,hostBfr1);
    }
    responder_aiErr = retrieve_addrinfo( respondername, &respond_addrP, &maxrespondercount, local_net_scope_id);

    if ( (enquirer_aiErr == 0) && (responder_aiErr == 0) )
    {
      if (verbose > 2) {
        hostBfr1[0] = hostBfr2[0] = '\0';
        if (maxenquirercount) {
          printline(stderr,"%Knum_enquirer_addrs= %d first at %p is %s\n"
                  ,enquire_addrP->ai_addr ,hostBfr1,maxenquirercount,(char *)enquire_addrP,hostBfr1);
        }
        if (maxrespondercount) {
          printline(stderr,"%Knum_responder_addrs= %d first at %p is %s\n"
                  ,respond_addrP->ai_addr ,hostBfr2
                  ,maxrespondercount,(char *)respond_addrP,hostBfr2);
        }
      }
      /*   acquire shared mem for supervisor-worker areas              */
      if ((shmkey=ftok(reliable_file[iamenquirer],job_identifier)) != -1)
      {
        shmsize = (size_t)(nworkers * ( sizeof(struct supvr_workr_common) + sizeof(pid_t)));
        /*   IF    we fail to shmget for reason of file (i.e. shm) already exists,
        **   AND   it is available to us by shmget (requires size no greater) and number of processes attached is zero
        **   THEN  re-use it on assumption that it was left orphaned from a kill-9'd run
        */
        if (    ( (rc = shmid = shmget(shmkey, shmsize, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)) == -1)
             && (errno == EEXIST)
           )
        {
          if (    ( (rc = shmid = shmget(shmkey, shmsize, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP)) != -1)
               && ( (rc = shmctl(shmid, IPC_STAT, &shmid_ds_buff)) == 0)
               && ( (shmid_ds_buff.shm_nattch) == 0)   /*   verify not in use !!  */
             )
          {
            printline(stderr,"%s: %s supervisor %d shmkey 0x%8.8X reusing shared-memory shmid %d size %d\n",command_nameP
                       ,(reportname+1),mypid,shmkey,shmid,shmsize);
          }
          else
          {
            printline(stderr,"%s: %s supervisor %d shmkey 0x%8.8X unable to reuse shared-memory shmid %d size %d for reason %s %d\n",command_nameP
                       ,(reportname+1),mypid,shmkey,shmid,shmsize
                       ,(rc ? "unable to acquire, errno=" : "still in use by number of other processes="),(rc ? errno : shmid_ds_buff.shm_nattch));
            rc = -1;
          }
        }
        if (rc != -1)
        {
          if ( (longrc = (long)(supvr_workrP = (struct supvr_workr_common *)(shmat(shmid, 0, 0)))) != -1)
          {
            /*  initialise shared mem and pid array                */
            /*  we initialise pidarray so that we can unambiguously tell whether element set to specific pid */
            memset((void *)supvr_workrP, 0, shmsize);
            pidarray = (pid_t *)(((char *)supvr_workrP) + (nworkers * ( sizeof(struct supvr_workr_common))));
            if (verbose > 1)
              printline(stderr,"%s: %s supervisor %d shmkey 0x%8.8X shmid %d size %d supvr_workrP %p pidarray %p\n",command_nameP
                       ,(reportname+1),mypid,shmkey,shmid,shmsize,supvr_workrP,pidarray);

            pidindex = 0;             /*   index of current worker   */

            /*    set our signal environment to invoke handlSIGS    */

            sigemptyset(&emptymask);
            sigemptyset(&blockusermask);
            sigemptyset(&blockalarmask);
            sigaddset(&blockusermask, SIGUSR1);
            sigaddset(&blockalarmask, SIGALRM);
            sigprocmask(SIG_SETMASK,&emptymask,0);       /*  normal mask allows USR1 */

            chsigaction.sa_handler = (void(*)(int))&handlSIGS;
            chsigaction.sa_mask = emptymask;
            chsigaction.sa_flags = 0;
            sigaction(SIGINT,   &chsigaction, &olsigINTaction);
            sigaction(SIGTERM,  &chsigaction, &olsigTRMaction);
            sigaction(SIGHUP,   &chsigaction, &olsigHUPaction);
            sigaction(SIGUSR1,  &chsigaction, &olsigUS1action);
            sigaction(SIGUSR2,  &chsigaction, &olsigUS2action);
            sigaction(SIGALRM,  &chsigaction, &olsigALRMaction);
            sigaction(SIGWINCH, &chsigaction, &olsigWCHaction);
#ifdef SIGPWR
            sigaction(SIGPWR,   &chsigaction, &olsigPWRaction);
#endif  /*  def SIGPWR  */
            sigaction(SIGXCPU,  &chsigaction, &olsigXCPaction);
            sigaction(SIG_DUMP_OPS,   &chsigaction, &olsigPREaction);
            sigaction(SIGPIPE,  &chsigaction, &olsigPIPaction);

            read_real_time(&start_time, TIMEBASE_SZ);
            /* initialise interval statistics */
            intvl_start_time.tb_high = start_time.tb_high;
            intvl_start_time.tb_low = start_time.tb_low;
            intvl_numsent = intvl_numrecv = intvl_nummsgw = intvl_nummsgr = 0;

            if (enquirerproccount > 0) /* if any work to do */
            {
            /*  call corresponding function to do the work  */
            rc = (iamenquirer ? enquirerfn() : responderfn());
            /*    supervisor returns here after forking all workers */
            /*    worker returns here too since exit loses output */
            if (supervisor_pid != getpid())  /*  if I'm worker */
              goto finish;

            curtime.tv_sec = 0;
            curtime.tv_usec = 0;
            curzone.tz_minuteswest = 0;
            curzone.tz_dsttime = 0;
            gettimeofday(&curtime,&curzone);
            if (rc)
            {
              printline(stderr,"%s: %s supervisor %d: error %d from enquirer/responder function at %24.24s %06.6d\n",command_nameP
                        ,(reportname+1),mypid, rc ,ctime((time_t *)&(curtime.tv_sec)), curtime.tv_usec);
            }
            else
            {
              if (!mimimal_out)
                printline(stdout,"%s %s supervisor %d: completed %d forks at %24.24s %06.6d\n",command_nameP
                        ,(reportname+1),mypid,nworkers ,ctime((time_t *)&(curtime.tv_sec)), curtime.tv_usec);

    set_interval_timer:
              /*  set our limit timer if specified by user
               *  Note  -  we used to set this for enquirer supervisor to expire at end of run
               *           but now we dont as the workers set their own.
               *           Now it is used only for intermediate intervals,
               *           both for enquirer and responder.
               */
              if ((nintervals > 1) && (interval_duration.it_value.tv_sec > 0))
              	setitimer ( ITIMER_REAL, &interval_duration, 0);

              /*   wait till all workers have terminated                                   */
    wait_for_workers:
              copy_of_signalled = signalled;             /* copy the global indicator ... */
              signalled = 0;                             /* ... and rapidly reset it  -  there's still a small timing window though */
              if  (copy_of_signalled == SIGALRM) {       /* time limit expired */
		nintervals--;
		if (nintervals > 0)                      /* but more intervals remain */
		      copy_of_signalled = SIG_DUMP_OPS;  /* so report on this one     */
		      goto display_current_interval;
	      }
              if (   (copy_of_signalled == SIGTERM)
                  || (copy_of_signalled == SIGINT)  /* interrupted (Ctl-C) */
                  || (copy_of_signalled == SIGALRM) /* time limit expired */
                 )
                goto kill_workers;
              waitrc = wait(0);
              if (waitrc == -1)  /*  no worker terminated */
              {
                waiterrno = errno;
                if (waiterrno == ECHILD)
                  goto all_workers_exited;
                /*  check for one of our signals  */
                  copy_of_signalled = signalled;             /*    copy the global indicator ... */
                  signalled = 0;                             /*    reset indicator               */
		  if  (copy_of_signalled == SIGALRM) {       /* time limit expired */
		    nintervals--;
		    if (nintervals > 0)                      /* but more intervals remain */
		      	  copy_of_signalled = SIG_DUMP_OPS;  /* so report on this one     */
			  goto display_current_interval;
		  }

		/*    SIG_DUMP_OPS requests display of statistics in current interval
		**    ( an interval starts with start-of-run and each SIG_DUMP_OPS )
                */
	display_current_interval:
                if (copy_of_signalled == SIG_DUMP_OPS) {
		  get_finish_time(RUSAGE_NONE, &intvl_start_time, &intvl_finish_time, &intvl_accum_time_base);
		  int_till_numsent = int_till_numrecv = int_till_nummsgw = int_till_nummsgr = 0;
		  for (ix = 0; ix < nworkers; ix++)
		    {
		      chshmp = supvr_workrP + ix;       /*  address supvr_workr_common area for this worker */
		      int_till_numsent += chshmp->ch_num_sent;
		      int_till_numrecv += chshmp->ch_num_recv;
		      int_till_nummsgw += chshmp->ch_num_msgw;
		      int_till_nummsgr += chshmp->ch_num_msgr;
		      if (chshmp->othrend_msg_blok_sz != supvr_workrP->othrend_msg_blok_sz)
			printline(stderr,"%s: %s worker index %d othrend_msg_blok_sz %u != worker index 0 othrend_msg_blok_sz %u\n",command_nameP
				  ,(reportname+1),ix,chshmp->othrend_msg_blok_sz,supvr_workrP->othrend_msg_blok_sz);
		    }

		    /*   adjust counters to within this interval  */
                    all_numsent = int_till_numsent - intvl_numsent;       all_numrecv = int_till_numrecv - intvl_numrecv;
                    all_nummsgw = int_till_nummsgw - intvl_nummsgw;       all_nummsgr = int_till_nummsgr - intvl_nummsgr;

		    printline(stdout,"netrush:" IPPROT_STRING ": %s %d k%d sent= %lu recv= %lu msgw= %lu msgr= %lu thissz %u othrsz %u\n"
			    " interval -%d- elapsed time %d.%" TVFORMAT "d end time %24.24s %06.6d\n"
			   ,(reportname+1), mypid, job_identifier, all_numsent, all_numrecv, all_nummsgw, all_nummsgr, MSG_BLOK_SZ, supvr_workrP->othrend_msg_blok_sz
			  ,nintervals, intvl_accum_time_base.tb_high, intvl_accum_time_base.tb_low
			  ,ctime((time_t *)&(curtime.tv_sec)), curtime.tv_usec
		    );

		    intvl_start_time.tb_high = intvl_finish_time.tb_high;
		    intvl_start_time.tb_low = intvl_finish_time.tb_low;
		    /*   note new start-of-interval counters  */
                    intvl_numsent = int_till_numsent;       intvl_numrecv = int_till_numrecv;
                    intvl_nummsgw = int_till_nummsgw;       intvl_nummsgr = int_till_nummsgr;

		    if (nintervals == nfyintvlnum) {  /*  send notification */
			curtime.tv_sec = 0;
			curtime.tv_usec = 0;
			curzone.tz_minuteswest = 0;
			curzone.tz_dsttime = 0;
			gettimeofday(&curtime,&curzone);
			nfyrc = nfyerrno = -999;
			charptr = 0;
		        if (  (sd = socket(nfysockad.sockaddr_u.opaque_sockaddr.sa_family, SOCK_DGRAM, IPPROTO_UDP)) >= 0  ) {
			    if (    (charptr = malloc(NOTIFYLEN)  )
				 && (  (*charptr = message_som),  (*(charptr+NOTIFYLEN-1) = message_eom),  (memset((charptr+1), stream_eof[0], NOTIFYLEN-2)  ))
			       ) {
                              int temprc;
                              for (ix = 0; ix < 5; ix++) {	 /* send five for luck */
			         temprc = sendto(sd, charptr, NOTIFYLEN, 0, ((const struct sockaddr *)&nfysockad), SOCKADDR_SIZE);
                                 if ((ix == 0) || (nfyrc >= 0)) { /*  set nfyrc on each successful send and also on first failing send if any */
			           nfyrc = temprc;
			           if (temprc < 0)
                                     nfyerrno = errno;
                                 }
                              }
			      if (nfyrc >= 0) {
                                if (verbose) {
                                  printline(stderr
                                            ,"%K%s: notified %s at %24.24s %06.6d\n"
                                            ,&nfysockad ,hostBfr1
                                            ,command_nameP,hostBfr1
                                            ,ctime((time_t *)&(curtime.tv_sec)), curtime.tv_usec
                                            );
                                }
			      }
			    }
			    if (nfyrc < 0) {
				printline(stderr
					  ,"%K%s: failed to notify %s, charptr= %p nfyrc= %d errno= %d at %24.24s %06.6d\n"
					  ,&nfysockad ,hostBfr1
					  ,command_nameP,hostBfr1
					  ,charptr, nfyrc ,nfyerrno
					  ,ctime((time_t *)&(curtime.tv_sec)), curtime.tv_usec
					  );
                            }
			    if (charptr)
			      free(charptr);
			}
			else
			    printline(stderr
				      ,"%s: failed to acquire notify socket, errno= %d at %24.24s %06.6d\n"
				      ,command_nameP,charptr ,errno
				      ,ctime((time_t *)&(curtime.tv_sec)), curtime.tv_usec
				      );
		    }
                }
		else
                if (   (copy_of_signalled == SIGTERM)
                    || (copy_of_signalled == SIGINT)  /* interrupted (Ctl-C) */
                    || (copy_of_signalled == SIGALRM) /* time limit expired */
                   )
                {
                kill_workers:
                  if (verbose > 1)
                  {
                    printline(stderr,"%s: %s supervisor %d about to kill all my workers\n",command_nameP,(reportname+1), mypid);
                  }
                  for (ix = 0; ix < nworkers; ix++)
                  {
                    if (   (pid = (*(pidarray+ix)))      /* verify a valid pid */
                        && (!(kill(pid,0)))           /* verify process is running */
                       )
                    {
                      if ((supvr_workrP + ix)->ch_flags & CH_CHILD_FINISHED)  /*  worker has finished all its work */
                      {
                        if (verbose > 1)
                        {
                          printline(stderr,"%s: %d index %d pid %d already finished\n",command_nameP,mypid,ix,pid);
                        }
                      }
                      else
                      {
                        if (rc = kill(pid,SIGTERM))  /* kill my worker */
                        {
                          rc = errno;
                          printline(stderr,"%s: %d error %d from attempt to kill %d %s\n",command_nameP,mypid, rc,pid,strerror(rc));
                        }
                        else if (verbose > 1)
                        {
                          printline(stderr,"%s: %d killed index %d pid %d\n",command_nameP,mypid,ix,pid);
                        }
                      }
                    }
                  }
                }

              }  /*  end waitrc == -1  */
              goto set_interval_timer;
            }
          all_workers_exited:

            get_finish_time(RUSAGE_CHILDREN, &start_time, &finish_time, &accum_time_base);
            all_numsent = all_numrecv = all_nummsgw = all_nummsgr = all_xsend_hwm = all_numselpol = 0; /*  init counters */
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
            all_numdynreg = all_numdynung = 0;       /*  number of calls to dynamically register/unregister events */
#endif /* POLLSET */
#if (POLLSET & POLLSET_EDGE_TRIG)    /*  count EAGAIN and EWOULDBLOCK for edge-triggered */
            all_numwdblck = 0;       /*  number of cases of EAGAIN and EWOULDBLOCK for edge-triggered */
#endif /*  (POLLSET & POLLSET_EDGE_TRIG) count EAGAIN and EWOULDBLOCK for edge-triggered */
            all_avg_send_len = all_avg_recv_len = 0.0;  /*  init averages  */
            for (ix = 0; ix < nworkers; ix++)
            {
              chshmp = supvr_workrP + ix;       /*  address supvr_workr_common area for this worker */
              all_numsent += chshmp->ch_num_sent;
              all_numrecv += chshmp->ch_num_recv;
              all_nummsgw += chshmp->ch_num_msgw;
              all_nummsgr += chshmp->ch_num_msgr;
              all_numselpol += chshmp->ch_num_selpol;
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
              all_numdynreg += chshmp->ch_num_dynreg;
              all_numdynung += chshmp->ch_num_dynung;
#endif /* POLLSET */
#if (POLLSET & POLLSET_EDGE_TRIG)    /*  count EAGAIN and EWOULDBLOCK for edge-triggered */
              all_numwdblck += chshmp->ch_num_wdblck;
#endif /*  (POLLSET & POLLSET_EDGE_TRIG) count EAGAIN and EWOULDBLOCK for edge-triggered */
              if (chshmp->ch_xsend_hwm > all_xsend_hwm)
                all_xsend_hwm = chshmp->ch_xsend_hwm;
              all_avg_send_len += ( (all_numsent > 0) ?
                                    (((double)(chshmp->ch_num_sent)) / ((double)(all_numsent))) : 0.0)
                                    * (chshmp->ch_avg_send_len - all_avg_send_len) ;
              all_avg_recv_len += ( (all_numrecv > 0) ?
                                    (((double)(chshmp->ch_num_recv)) / ((double)(all_numrecv))) : 0.0)
                                    * (chshmp->ch_avg_recv_len - all_avg_recv_len);
              if (chshmp->ch_pid != (*(pidarray+ix)))
                printline(stderr,"%s: %s worker index %d pidarray pid %d != shm pid %d\n",command_nameP
                          ,(reportname+1),ix,(*(pidarray+ix)),chshmp->ch_pid);
              if (chshmp->othrend_msg_blok_sz != supvr_workrP->othrend_msg_blok_sz)
                printline(stderr,"%s: %s worker index %d othrend_msg_blok_sz %u != worker index 0 othrend_msg_blok_sz %u\n",command_nameP
                          ,(reportname+1),ix,chshmp->othrend_msg_blok_sz,supvr_workrP->othrend_msg_blok_sz);
                printline(((verbose > 2) ? stdout : 0),"%s: %s worker %d sent= %lu avg_len= %.2f recv= %lu avg_len= %.2f msgw= %lu msgr= %lu xsend_hwm= %lu "
#ifdef POLLSET
                     "npolls"
#else /*  select */
                     "nselects"
#endif /*  select */
                     "= %lu"
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
                    " ndynreg %lu ndynung %lu"
#endif /* POLLSET */
#if (POLLSET & POLLSET_EDGE_TRIG)    /*  count EAGAIN and EWOULDBLOCK for edge-triggered */
                  " nwdblck %lu"
#endif /*  (POLLSET & POLLSET_EDGE_TRIG) count EAGAIN and EWOULDBLOCK for edge-triggered */
                         "\n"
                          ,command_nameP
                          ,(reportname+1),chshmp->ch_pid,chshmp->ch_num_sent,chshmp->ch_avg_send_len,chshmp->ch_num_recv,chshmp->ch_avg_recv_len,chshmp->ch_num_msgw,chshmp->ch_num_msgr,chshmp->ch_xsend_hwm, chshmp->ch_num_selpol
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
                          ,chshmp->ch_num_dynreg ,chshmp->ch_num_dynung
#endif /* POLLSET */
#if (POLLSET & POLLSET_EDGE_TRIG)    /*  count EAGAIN and EWOULDBLOCK for edge-triggered */
                          ,chshmp->ch_num_wdblck
#endif /*  (POLLSET & POLLSET_EDGE_TRIG) count EAGAIN and EWOULDBLOCK for edge-triggered */
                         );
            }

            adj_numsent = ((double)(all_numsent)) * ( all_avg_send_len / (double) MSG_BLOK_SZ );
            adj_numrecv = ((double)(all_numrecv)) * ( all_avg_recv_len / (double) supvr_workrP->othrend_msg_blok_sz );

            printline((verbose ? stdout :0),"%s:" IPPROT_STRING ": %s %d k%d terminating after %d workers sent= %lu avg_len= %.2f adj_sent= %.2f recv= %lu avg_len= %.2f adj_recv= %.2f msgw= %lu msgr= %lu thissz %u othrsz %u xsend_hwm= %lu "
#ifdef POLLSET
                     "npolls"
#else /*  select */
                     "nselects"
#endif /*  select */
                     "= %lu "
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
                    "ndynreg %lu ndynung %lu "
#endif /* POLLSET */
#if (POLLSET & POLLSET_EDGE_TRIG)    /*  count EAGAIN and EWOULDBLOCK for edge-triggered */
                  "nwdblck %lu "
#endif /*  (POLLSET & POLLSET_EDGE_TRIG) count EAGAIN and EWOULDBLOCK for edge-triggered */
                    "elapsed time %d.%" TVFORMAT "d end time %24.24s %06.6d\n user CPU %d.%06.6d, system CPU %d.%06.6d, u:s %.3f\n vol contxt swchs %d, invol contxt swchs %d\n"
              ,command_nameP
              ,(reportname+1), mypid, job_identifier, nworkers, all_numsent, all_avg_send_len
		      ,adj_numsent, all_numrecv, all_avg_recv_len, adj_numrecv, all_nummsgw, all_nummsgr, MSG_BLOK_SZ, supvr_workrP->othrend_msg_blok_sz, all_xsend_hwm ,all_numselpol
#if (POLLSET & POLLSET_DYNAMIC_REG)  /*   if we are to dynamically unregister/register pollout events */
              ,all_numdynreg ,all_numdynung
#endif /* POLLSET */
#if (POLLSET & POLLSET_EDGE_TRIG)    /*  count EAGAIN and EWOULDBLOCK for edge-triggered */
              ,all_numwdblck
#endif /*  (POLLSET & POLLSET_EDGE_TRIG) count EAGAIN and EWOULDBLOCK for edge-triggered */
              ,accum_time_base.tb_high, accum_time_base.tb_low
              ,ctime((time_t *)&(curtime.tv_sec)), curtime.tv_usec
              ,RUsage.ru_utime.tv_sec, RUsage.ru_utime.tv_usec
              ,RUsage.ru_stime.tv_sec, RUsage.ru_stime.tv_usec
	      ,(       ( (RUsage.ru_stime.tv_sec > 0) || (RUsage.ru_stime.tv_usec > 0) )
		 ?     (   ((double)RUsage.ru_utime.tv_sec + ((double)RUsage.ru_utime.tv_usec / (double)PERSEC))
                         / ((double)RUsage.ru_stime.tv_sec + ((double)RUsage.ru_stime.tv_usec / (double)PERSEC))
       	               )
		 : -1.0
       	       )
              ,RUsage.ru_nvcsw, RUsage.ru_nivcsw
            );

            if (tabular)
            {
              gettimeofday(&curtime,&curzone);
              printline(stdout,"\n%24.24s %03.3d %c %7d %6ld %6ld %6ld %c %c %c "
                               "%3d "   /*   XSEND_LIMIT   */
                               "%3d "   /*   XRECV_LIMIT   */
                               "%6d %6d"
                               " %9lu %9lu %9lu %9lu %9u "
                               "%9lu %9lu "
                               "%d.%" TVFORMAT "d"
                               " %d.%06.6d %d.%06.6d\n"
                ,ctime((time_t *)&(curtime.tv_sec)), (curtime.tv_usec / 1000)
                ,specie[iamenquirer][0],mypid,enquirerproccount ,responderproccount ,max_sendcount
                ,(tcp_rfc1323 ? 'R' : '-') ,(tcp_nodelay ? 'N' : '-') ,
                 (msg_waitall ? 'W' : '-')
                ,XSEND_LIMIT
                ,
#ifdef POLLSET
		-(POLLSET)
#else /* not POLLSET */
		 -97
#endif /* not POLLSET */
                ,sosndbufsz ,sorcvbufsz
                ,all_numsent ,all_numrecv ,all_nummsgw ,all_nummsgr, supvr_workrP->othrend_msg_blok_sz
                ,0,0
                ,accum_time_base.tb_high ,accum_time_base.tb_low
                ,RUsage.ru_utime.tv_sec ,RUsage.ru_utime.tv_usec ,RUsage.ru_stime.tv_sec ,RUsage.ru_stime.tv_usec);
            }
            }
	    
            /*     now restore previous signal action             */
            sigaction(SIGUSR1, &olsigUS1action, NULL);
            sigaction(SIGUSR2, &olsigUS2action, NULL);
            sigaction(SIGALRM, &olsigALRMaction, NULL);
            sigaction(SIGWINCH, &olsigWCHaction, NULL);
            sigaction(SIGXCPU, &olsigXCPaction, NULL);
#ifdef SIGPWR
            sigaction(SIGPWR, &olsigPWRaction, NULL);
#endif  /*  def SIGPWR  */
            sigaction(SIG_DUMP_OPS, &olsigPREaction, NULL);
            sigaction(SIGPIPE, &olsigPIPaction, NULL);
            sigaction(SIGINT, &olsigINTaction, NULL);
            sigaction(SIGTERM, &olsigTRMaction, NULL);
            sigaction(SIGHUP, &olsigHUPaction, NULL);
          }
          else
          {
            rc = errno;
            printline(stderr, "%s: %s supervisor %d shmat of shmid %d failed rc %ld errno %d %s\n",command_nameP
                              ,(reportname+1),mypid,shmid,(long)supvr_workrP,rc, strerror(rc));
          }

          if ( (shmctl(shmid, IPC_RMID, &shmid_ds_buff)) == -1)
          {
	    printline(stderr, "%s: %s supervisor %d shmctl RMID of %d failed errno %d %s\n",command_nameP,(reportname+1),mypid,shmid,errno, strerror(errno));
          }
	  
        }
        else
        {
          rc = errno;
          printline(stderr, "%s: %s supervisor %d shmget of size %lu failed errno %d %s\n",command_nameP
                            ,(reportname+1),mypid, shmsize,rc, strerror(rc));
        }
      }
      else
      {
        rc = errno;
        printline(stderr, "%s: %s supervisor %d unable to obtain key for shared memory errno %d %s\n",command_nameP
                          ,(reportname+1),mypid,rc, strerror(rc));
      }
    }
    if (enquire_addrP)
      free(enquire_addrP);
    if (respond_addrP)
      free(respond_addrP);
    if (nfy_addrinf_p)
      free(nfy_addrinf_p);
  }
  else
  {
    print_version();
    if (wantshelp)
      fprintf(stderr,usage_text,command_nameP);
    if (rc == 0)
        rc = 15;
  }

  if (nfy_host_name)
    free(nfy_host_name);

  if (reportFILE)
  {
    fclose (reportFILE);
    reportFILE = (FILE*)0;  /*  note cannot write to it any more */
  }
#ifdef SLEEP_NETRUSH_AT_END
#if (SLEEP_NETRUSH_AT_END == 1)
#undef SLEEP_NETRUSH_AT_END
#define SLEEP_NETRUSH_AT_END 300
#endif
  sleep(SLEEP_NETRUSH_AT_END);
#endif

 freestg:
#ifdef DEBUG_NETRUSH
 printline(stderr,"%s: about to free enquirername %p\n",command_nameP,enquirername);
#endif
  if (enquirername)  free(enquirername);

 finish:
  return rc;
}
#endif /*  not MSG_OUTPUT_TO_FILE or DYNAMIC_MSG_BUF == 4096 */
