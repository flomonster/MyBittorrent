ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = my-bittorrent

my_bittorrent_LDADD = -lcurl -lcrypto
my_bittorrent_SOURCES = src/bencode.c src/bencode_utils.c src/bencode_print.c \
                        src/dbuf.c includes/bencode.h \
                        includes/dbuf.h includes/file.h includes/filelist.h \
                        includes/piece.h src/bittorrent.c \
                        src/btopts.c includes/btopts.h includes/fmap.h \
                        src/fmap.c includes/metainfo.h src/metainfo.c \
                        src/peer.c includes/peer.h \
                        src/peerlist.c includes/peerlist.h \
                        src/torrent.c includes/torrent.h \
                        src/tracker.c includes/tracker.h \
                        src/server.c includes/server.h \
                        src/file.c includes/file.h \
                        src/filelist.c includes/filelist.h \
                        src/path.c includes/path.h \
                        src/announce.c includes/announce.h \
                        src/log.c src/log.h \
                        src/bitset.c includes/bitset.h

AM_CPPFLAGS = -iquote includes -D_POSIX_C_SOURCE=200809L
AM_CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
