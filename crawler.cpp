#include "crawler.h"
#include "JsonHandle.h"
#include "core.h"
#include "Client.h"
#include "request_data.h"

using std::vector;
using std::cout;
using std::string;

void crawler_init(SaoFU::Client& bot) {

    auto baha_data = SaoFU::data_pack("baha", u8"搜尋巴哈動畫")
                     .set_more_help(u8"|baha [搜尋]")
                     .set_argc(SaoFU::param_variadic(1));

    bot.add_command(baha_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        string find_anime = "";
        auto msg_id = bot.command->sendMessage(ctx.channelID, argv[1] + u8"\n進度: 0%");
        int lim = 4;
        for (int i = 1; i <= lim; i++) {
            bot.command->editMessage(msg_id,
                argv[1] + u8"\n進度: " + std::to_string(float(i / lim) * 100) + u8"%");
            string url = "https://ani.gamer.com.tw/animeList.php?page=" + std::to_string(i);
            SaoFU::UrlHandle request(url);
            request.urlopen();

            vector<string> url_data;
            vector<string> video_url = request.search("theme-list-main");
            for (auto& row : video_url) {
                string spilit = SaoFU::string_split(row, "\'")[1];
                url_data.emplace_back("https://ani.gamer.com.tw/" + spilit);
            }

            vector<string> name = request.search("theme-name");
            for (auto& row : SaoFU::enumerate(name)) {
                string anime_name = SaoFU::string_split(row.second, "<", ">");
                if (anime_name.find(argv[1]) != string::npos) {
                    find_anime = find_anime + anime_name + "\n" + url_data[row.first] + "\n";
                }
            }
        }
        if (find_anime.empty()) find_anime = u8"沒有搜尋結果";
        bot.command->editMessage(msg_id, find_anime);
    });

    auto weather_data = SaoFU::data_pack("weather", u8"天氣預報")
                        .set_more_help(u8"|weather [地點]")
                        .set_argc(SaoFU::param_fixed(1));

    bot.add_command(weather_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        SaoFU::UrlHandle request(
            "https://opendata.cwb.gov.tw/api/v1/rest/datastore/" + bot.get_path[u8"weather_pwd"].get<string>() +
            "&format=JSON");
        string search_city = argv[1];
        auto w_json = SaoFU::JsonHandle::parse(request.urlopen().data);

        if (!w_json.is_json) {
            bot.command->sendMessage(ctx.channelID, u8"發生了一些小錯誤\n請再次嘗試指令");
            SaoFU::Logger(w_json.str);
            return;
        }

        string city = "";
        string content = u8"城市名稱錯誤，請確認是否打成 ｢台｣";
        string title[] = {u8"", u8"降雨機率: ", u8"最低溫度: ", u8"體感: ", u8"最高溫度: "};
        string unit[] = {u8"", u8"%", u8"°C", u8"", u8"°C"};
        for (auto& weather_row : w_json[u8"records"][u8"location"].json()) {
            city = weather_row[u8"locationName"];
            if (city.find(search_city) != string::npos) {
                content = city + " " + string(weather_row[u8"weatherElement"][0][u8"time"][0][u8"startTime"]) + "\n";
                for (auto& row : SaoFU::enumerate(weather_row[u8"weatherElement"])) {
                    content += title[row.first] + string(row.second[u8"time"][0][u8"parameter"][u8"parameterName"]) +
                        unit[row.first] + "\n";
                }
            }
        }
        bot.command->sendMessage(ctx.channelID, content);
    });

    bot.add_command(u8"top##顯示熱門新聞", [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        SaoFU::UrlHandle
            request("https://newsapi.org/v2/top-headlines?country=tw&apiKey=" + bot.get_path[u8"news_pwd"].get<string>());
        Json nw_json = Json::parse(request.urlopen().data);
        string nw_str = "";
        for (auto& row : SaoFU::enumerate(nw_json["articles"])) {
            string title = row.second["title"];
            nw_str += std::to_string(row.first + 1) + ". " + title + "\n";
        }
        bot.command->sendMessage(ctx.channelID, nw_str);
    });

    auto news_data = SaoFU::data_pack("news", u8"搜尋新聞")
                     .set_more_help(u8"|news [搜尋]")
                     .set_argc(SaoFU::param_variadic(1));

    bot.add_command(news_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        string url = "https://newsapi.org/v2/everything?language=zh&q=" + SaoFU::url_encode(argv[1]) +
            "&apiKey=" + bot.get_path[u8"news_pwd"].get<string>();
        SaoFU::UrlHandle request(url);
        Json nw_json = Json::parse(request.urlopen().data);
        int rand_gen = SaoFU::rand_generate(nw_json[u8"articles"].size());
        if (rand_gen > 0) {
            bot.command->sendMessage(ctx.channelID, nw_json[u8"articles"][rand_gen][u8"url"]);
        }
        else {
            bot.command->sendMessage(ctx.channelID, u8"查無關鍵字");
        }
    });
}
