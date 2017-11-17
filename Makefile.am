ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = my-bittorrent

my_bittorrent_LDADD = -lcurl -lcrypto
my_bittorrent_SOURCES = src/bencode.c src/bencode_utils.c src/bencode_print.c \
                        src/dbuf.c includes/bencode.h \
                        includes/dbuf.h includes/file.h includes/filelist.h \
                        includes/peer.h includes/piece.h src/bittorrent.c \
                        src/btopts.c includes/btopts.h includes/fmap.h \
                        src/fmap.c includes/metainfo.h src/metainfo.c \
                        src/torrent.c includes/torrent.h \
                        src/tracker.c includes/tracker.h \
												src/server.c includes/server.h
AM_CPPFLAGS = -iquote includes
