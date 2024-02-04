/* radios.json, exemple :
[
	{"name":"Fip","url":"http://icecast.radiofrance.fr/fip-hifi.aac"},
	{"name":"Fip Jazz","url":"http://icecast.radiofrance.fr/fipjazz-hifi.aac"},
	{"name":"France Musique","url":"http://icecast.radiofrance.fr/francemusique-hifi.aac"},
	{"name":"Bossa Nova","url":"http://185.33.21.112/bossanova_128"},
	{"name":"The Wave","url":"http://75.102.53.58/1066"},
	{"name":"The Lounge","url":"http://64.95.243.43:8020/stream"},
	{"name":"Funky Radio","url":"http://176.31.111.65:4744"},
	{"name":"80s","url":"http://ice1.somafm.com/u80s-128-mp3" },
	{"name":"Classic","url":"http://media-ice.musicradio.com/ClassicFMMP3"},
	{"name":"Night Ride","url":"http://stream.nightride.fm/nightride.mp3"}
]
*/

struct WebRadios {
  unsigned int max;
  char name[MAX_RADIOS][64];
  char url[MAX_RADIOS][127];
};

void fillWebRadiosFromJson(JSONVar json, WebRadios* webradios) {
  int size = json.length();
  for (int i = 0; i < size && i < MAX_RADIOS; i++) {
    sprintf(webradios->name[i], "%s", (const char*) json[i]["name"]);
    sprintf(webradios->url[i], "%s", (const char*) json[i]["url"]);
    webradios->max = i + 1;
  }
}