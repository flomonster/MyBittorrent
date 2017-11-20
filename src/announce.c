#include <err.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "announce.h"
#include "dbuf.h"
#include "log.h"
#include "metainfo.h"
#include "torrent.h"


static size_t buf_write_callback(void *contents, size_t wsize,
                                 size_t nmemb, void *userp)
{
  wsize *= nmemb;
  s_dbuf *buf = userp;

  if (!(buf->data = realloc(buf->data, buf->size + wsize + 1)))
  {
    warn("realloc returned NULL");
    return 0;
  }

  memcpy(&(buf->data[buf->size]), contents, wsize);
  buf->size += wsize;
  buf->data[buf->size] = 0;

  return wsize;
}


static s_dbuf *tracker_announce_url(CURL *curl, char *url)
{
  CURLcode res;
  s_dbuf *buf = dbuf_create(0);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, buf_write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);

  res = curl_easy_perform(curl);

  if (res == CURLE_OK)
    return buf;

  warnx("curl_easy_perform() failed: %s",
        curl_easy_strerror(res));

  dbuf_free(buf);
  return NULL;
}


static s_dbuf *tracker_announce_sub(CURL *curl, s_torrent *tor)
{
  char url[2000];
  char *info_hash = curl_easy_escape(curl, tor->metainfo.sha,
                                     SHA_DIGEST_LENGTH);
  sprintf(url, "%s?peer_id=%.20s"
          "&info_hash=%s"
          "&port=6881&left=1501102080&downloaded=100"
          "&uploaded=0&compact=1&event=started",
          tor->tracker.url, tor->peer_id, info_hash);
  curl_free(info_hash);
  return tracker_announce_url(curl, url);
}


s_dbuf *tracker_announce_raw(s_torrent *tor)
{
  LOG(L_SNETDBG, "tracker", tor, "requesting peers to %s", tor->tracker.url);
  CURL *curl = curl_easy_init();
  if (!curl)
  {
    warnx("could not initialize curl");
    return NULL;
  }

  s_dbuf *res = tracker_announce_sub(curl, tor);
  curl_easy_cleanup(curl);
  return res;
}


s_announce *tracker_announce(s_torrent *tor)
{
  s_dbuf *raw = tracker_announce_raw(tor);
  if (!raw)
    return NULL;

  s_bdata *bencoded = bencode_parse(raw);
  if (!bencoded)
  {
    dbuf_free(raw);
    return NULL;
  }

  s_announce *ret = malloc(sizeof(*ret));
  if (!ret)
    return NULL;

  ret->raw = raw;
  ret->bencoded = bencoded;
  return ret;
}


void announce_free(s_announce *announce)
{
  if (!announce)
    return;

  dbuf_free(announce->raw);
  bencode_free(announce->bencoded);
  free(announce);
}
