#include "../Spotify.Tracker/include/utilities.h"

char* json_data = "{"
    "\"timestamp\" : 1725204061547,"
    "\"context\" : {"
        "\"external_urls\" : {"
            "\"spotify\" : \"https://open.spotify.com/playlist/6h18aDpBtRmTEouUn5FafG\""
        "},"
        "\"href\" : \"https://api.spotify.com/v1/playlists/6h18aDpBtRmTEouUn5FafG\","
        "\"type\" : \"playlist\","
        "\"uri\" : \"spotify:playlist:6h18aDpBtRmTEouUn5FafG\""
    "},"
    "\"progress_ms\" : 110564,"
    "\"item\" : {"
        "\"album\" : {"
            "\"album_type\" : \"album\","
            "\"artists\" : [ {"
                "\"external_urls\" : {"
                    "\"spotify\" : \"https://open.spotify.com/artist/5k3NfhEeZHpouIGDpjKOPo\""
                "},"
                "\"href\" : \"https://api.spotify.com/v1/artists/5k3NfhEeZHpouIGDpjKOPo\","
                "\"id\" : \"5k3NfhEeZHpouIGDpjKOPo\","
                "\"name\" : \"Shiro SAGISU\","
                "\"type\" : \"artist\","
                "\"uri\" : \"spotify:artist:5k3NfhEeZHpouIGDpjKOPo\""
            "} ],"
            "\"available_markets\" : [ \"AR\", \"AU\", \"AT\", \"BE\", \"BO\", \"BR\", \"BG\", \"CA\", \"CL\", \"CO\", \"CR\", \"CY\", \"CZ\", \"DK\", \"DO\", \"DE\", \"EC\", \"EE\", \"SV\", \"FI\", \"FR\", \"GR\", \"GT\", \"HN\", \"HK\", \"HU\", \"IS\", \"IE\", \"IT\", \"LV\", \"LT\", \"LU\", \"MY\", \"MT\", \"MX\", \"NL\", \"NZ\", \"NI\", \"NO\", \"PA\", \"PY\", \"PE\", \"PH\", \"PL\", \"PT\", \"SG\", \"SK\", \"ES\", \"SE\", \"CH\", \"TW\", \"TR\", \"UY\", \"US\", \"GB\", \"AD\", \"LI\", \"MC\", \"ID\", \"JP\", \"TH\", \"VN\", \"RO\", \"IL\", \"ZA\", \"SA\", \"AE\", \"BH\", \"QA\", \"OM\", \"KW\", \"EG\", \"MA\", \"DZ\", \"TN\", \"LB\", \"JO\", \"PS\", \"IN\", \"BY\", \"KZ\", \"MD\", \"UA\", \"AL\", \"BA\", \"HR\", \"ME\", \"MK\", \"RS\", \"SI\", \"KR\", \"BD\", \"PK\", \"LK\", \"GH\", \"KE\", \"NG\", \"TZ\", \"UG\", \"AG\", \"AM\", \"BS\", \"BB\", \"BZ\", \"BT\", \"BW\", \"BF\", \"CV\", \"CW\", \"DM\", \"FJ\", \"GM\", \"GE\", \"GD\", \"GW\", \"GY\", \"HT\", \"JM\", \"KI\", \"LS\", \"LR\", \"MW\", \"MV\", \"ML\", \"MH\", \"FM\", \"NA\", \"NR\", \"NE\", \"PW\", \"PG\", \"PR\", \"WS\", \"SM\", \"ST\", \"SN\", \"SC\", \"SL\", \"SB\", \"KN\", \"LC\", \"VC\", \"SR\", \"TL\", \"TO\", \"TT\", \"TV\", \"VU\", \"AZ\", \"BN\", \"BI\", \"KH\", \"CM\", \"TD\", \"KM\", \"GQ\", \"SZ\", \"GA\", \"GN\", \"KG\", \"LA\", \"MO\", \"MR\", \"MN\", \"NP\", \"RW\", \"TG\", \"UZ\", \"ZW\", \"BJ\", \"MG\", \"MU\", \"MZ\", \"AO\", \"CI\", \"DJ\", \"ZM\", \"CD\", \"CG\", \"IQ\", \"LY\", \"TJ\", \"VE\", \"ET\", \"XK\" ],"
            "\"external_urls\" : {"
                "\"spotify\" : \"https://open.spotify.com/album/0qmiu9xV3gQ0N3R4xOx62N\""
            "},"
            "\"href\" : \"https://api.spotify.com/v1/albums/0qmiu9xV3gQ0N3R4xOx62N\","
            "\"id\" : \"0qmiu9xV3gQ0N3R4xOx62N\","
            "\"images\" : [ {"
                "\"height\" : 640,"
                "\"url\" : \"https://i.scdn.co/image/ab67616d0000b273cbd142c2e1dd14068e5a270d\","
                "\"width\" : 640"
            "}, {"
                "\"height\" : 300,"
                "\"url\" : \"https://i.scdn.co/image/ab67616d00001e02cbd142c2e1dd14068e5a270d\","
                "\"width\" : 300"
            "}, {"
                "\"height\" : 64,"
                "\"url\" : \"https://i.scdn.co/image/ab67616d00004851cbd142c2e1dd14068e5a270d\","
                "\"width\" : 64"
            "} ],"
            "\"name\" : \"TV Animation BLEACH THE BLOOD WARFARE Original Soundtrack I\","
            "\"release_date\" : \"2023-12-17\","
            "\"release_date_precision\" : \"day\","
            "\"total_tracks\" : 27,"
            "\"type\" : \"album\","
            "\"uri\" : \"spotify:album:0qmiu9xV3gQ0N3R4xOx62N\""
        "},"
        "\"artists\" : [ {"
            "\"external_urls\" : {"
                "\"spotify\" : \"https://open.spotify.com/artist/5k3NfhEeZHpouIGDpjKOPo\""
            "},"
            "\"href\" : \"https://api.spotify.com/v1/artists/5k3NfhEeZHpouIGDpjKOPo\","
            "\"id\" : \"5k3NfhEeZHpouIGDpjKOPo\","
            "\"name\" : \"Shiro SAGISU\","
            "\"type\" : \"artist\","
            "\"uri\" : \"spotify:artist:5k3NfhEeZHpouIGDpjKOPo\""
        "} ],"
        "\"available_markets\" : [ \"AR\", \"AU\", \"AT\", \"BE\", \"BO\", \"BR\", \"BG\", \"CA\", \"CL\", \"CO\", \"CR\", \"CY\", \"CZ\", \"DK\", \"DO\", \"DE\", \"EC\", \"EE\", \"SV\", \"FI\", \"FR\", \"GR\", \"GT\", \"HN\", \"HK\", \"HU\", \"IS\", \"IE\", \"IT\", \"LV\", \"LT\", \"LU\", \"MY\", \"MT\", \"MX\", \"NL\", \"NZ\", \"NI\", \"NO\", \"PA\", \"PY\", \"PE\", \"PH\", \"PL\", \"PT\", \"SG\", \"SK\", \"ES\", \"SE\", \"CH\", \"TW\", \"TR\", \"UY\", \"US\", \"GB\", \"AD\", \"LI\", \"MC\", \"ID\", \"JP\", \"TH\", \"VN\", \"RO\", \"IL\", \"ZA\", \"SA\", \"AE\", \"BH\", \"QA\", \"OM\", \"KW\", \"EG\", \"MA\", \"DZ\", \"TN\", \"LB\", \"JO\", \"PS\", \"IN\", \"BY\", \"KZ\", \"MD\", \"UA\", \"AL\", \"BA\", \"HR\", \"ME\", \"MK\", \"RS\", \"SI\", \"KR\", \"BD\", \"PK\", \"LK\", \"GH\", \"KE\", \"NG\", \"TZ\", \"UG\", \"AG\", \"AM\", \"BS\", \"BB\", \"BZ\", \"BT\", \"BW\", \"BF\", \"CV\", \"CW\", \"DM\", \"FJ\", \"GM\", \"GE\", \"GD\", \"GW\", \"GY\", \"HT\", \"JM\", \"KI\", \"LS\", \"LR\", \"MW\", \"MV\", \"ML\", \"MH\", \"FM\", \"NA\", \"NR\", \"NE\", \"PW\", \"PG\", \"PR\", \"WS\", \"SM\", \"ST\", \"SN\", \"SC\", \"SL\", \"SB\", \"KN\", \"LC\", \"VC\", \"SR\", \"TL\", \"TO\", \"TT\", \"TV\", \"VU\", \"AZ\", \"BN\", \"BI\", \"KH\", \"CM\", \"TD\", \"KM\", \"GQ\", \"SZ\", \"GA\", \"GN\", \"KG\", \"LA\", \"MO\", \"MR\", \"MN\", \"NP\", \"RW\", \"TG\", \"UZ\", \"ZW\", \"BJ\", \"MG\", \"MU\", \"MZ\", \"AO\", \"CI\", \"DJ\", \"ZM\", \"CD\", \"CG\", \"IQ\", \"LY\", \"TJ\", \"VE\", \"ET\", \"XK\" ],"
        "\"disc_number\" : 1,"
        "\"duration_ms\" : 160222,"
        "\"explicit\" : false,"
        "\"external_ids\" : {"
            "\"isrc\" : \"JPB602300715\""
        "},"
        "\"external_urls\" : {"
            "\"spotify\" : \"https://open.spotify.com/track/1GAqZlCMlOFToQ8uZl6e6o\""
        "},"
        "\"href\" : \"https://api.spotify.com/v1/tracks/1GAqZlCMlOFToQ8uZl6e6o\","
        "\"id\" : \"1GAqZlCMlOFToQ8uZl6e6o\","
        "\"is_local\" : false,"
        "\"name\" : \"RUTHLESS\","
        "\"popularity\" : 45,"
        "\"preview_url\" : \"https://p.scdn.co/mp3-preview/9f1f469c4e407bd61fd2fdab6a9c0c606c9d5aa2?cid=774b29d4f13844c495f206cafdad9c86\","
        "\"track_number\" : 4,"
        "\"type\" : \"track\","
        "\"uri\" : \"spotify:track:1GAqZlCMlOFToQ8uZl6e6o\""
    "},"
    "\"currently_playing_type\" : \"track\","
    "\"is_playing\" : true"
"}";

int main(void) {
    char *str = get_str_between(json_data, "\"is_local\" :", "\"popularity");
    char *str_2 = get_str_between(str, "\"name\" :", ",");
    printf("%s\n", str);
    printf("%s\n", str_2);
    exit(1);
}