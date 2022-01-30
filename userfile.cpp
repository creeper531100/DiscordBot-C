#include "userfile.h"
#include "JsonHandle.h"
#include "request_data.h"
#include "core.h"

using std::vector;
using std::cout;
using std::string;

class Permutations {
public:
    vector<string> json_sentence;
    Json json;

    Permutations(string path) {
        json = SaoFU::JsonHandle(path).json();
        vector<string> this_json_sentence = this->json[u8"詞語"];
        json_sentence = this_json_sentence;
    }

    string rand() {
        return regroup(SaoFU::rand_generate(json_sentence));
    }

    string origin() {
        return SaoFU::rand_generate(json_sentence);
    }

    string search_sentence(string name) {
        auto sentence_obj = SaoFU::find<SaoFU::SEARCH_CONTENT>(name).in(json_sentence);
        if (sentence_obj.is_true)
            return regroup(sentence_obj.data<const char*>());
        return "None";
    }

    string set_rand_range(int min, int max) {
        int rand_range = SaoFU::rand_generate(min, max);
        return regroup(json[u8"詞語"][rand_range]);
    }

    string regroup(string select) {
        string sentence = "";
        vector<string> sentence_element;
        vector<string> split = SaoFU::string_split(select, ";");
        SaoFU::transform_item(json[u8"行為"], sentence_element);
        for (auto& row : split) {
            if (SaoFU::find(row).in(sentence_element).is_true) {
                vector<string> element = json[u8"行為"][row];
                sentence += SaoFU::rand_generate(element);
            }
            else
                sentence += row;
        }
        return sentence;
    }
};

void userfile_init(SaoFU::Client& bot) {
    Permutations* permutations = new Permutations(bot.get_path[u8"redstone_HDMI"]);

    auto rand_data = SaoFU::data_pack("rand", u8"隨機屁話")
                     .set_more_help(u8"|rand [無須參數]")
                     .set_emoji(u8"❔");

    bot.add_command(rand_data, [&bot, permutations](SleepyDiscord::Message ctx, vector<string> argv) {
        string user_name = ctx.author.username;
        if (bot.is_emoji)
            user_name = argv[0];
        else
            bot.command->deleteMessage(ctx.channelID, ctx.ID);
        bot.command->sendMessage(ctx.channelID, user_name + ": " + permutations->rand());
    });

    auto findrand_data = SaoFU::data_pack("findrand", u8"尋找組合句")
                         .set_more_help(u8"|findrand [組合句]")
                         .set_argc(SaoFU::param_variadic(1));

    bot.add_command(findrand_data, [&bot, permutations](SleepyDiscord::Message ctx, vector<string> argv) {
        string user_name = ctx.author.username;
        bot.command->deleteMessage(ctx.channelID, ctx.ID);
        bot.command->sendMessage(ctx.channelID, user_name + ": " + permutations->search_sentence(argv[1]));
    });

    bot.add_command(u8"lastrand##顯示最新組合句", [&bot, permutations](SleepyDiscord::Message ctx, vector<string> argv) {
        string user_name = ctx.author.username;
        bot.command->deleteMessage(ctx.channelID, ctx.ID);
        bot.command->sendMessage(ctx.channelID, user_name + ": " +
            permutations->set_rand_range(permutations->json_sentence.size() - 5, permutations->json_sentence.size() - 1));
    });

    auto randimg_data = SaoFU::data_pack("randimg", u8"產生隨機圖片")
                        .set_more_help(u8"open_file")
                        .set_emoji(u8"🌛");

    bot.add_command(randimg_data, [&bot, permutations](SleepyDiscord::Message ctx, vector<string> argv) {
        std::ofstream ofs(bot.get_path[u8"ofs"].get<string>());
        string content = "";

        if (bot.is_emoji || argv[1].empty())
            content = permutations->rand();
        else
            content = argv[1];

        ofs << content;
        ofs.close();
        system(bot.get_path[u8"pic_handle"].get<string>().c_str());
        try {
            bot.command->uploadFile(ctx.channelID, bot.get_path[u8"image0"], u8"");
        }
        catch (SleepyDiscord::ErrorCode e) {
            bot.command->sendMessage(ctx.channelID, u8"圖片太大");
        }
    });

    auto motto_data = SaoFU::data_pack("motto", u8"顯示金句")
                      .set_emoji(u8"ℹ️")
                      .set_more_help("|motto");

    bot.add_command(motto_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        Json motto_json = SaoFU::JsonHandle(bot.get_path[u8"say"]).json();
        vector<string> json_vec = motto_json[u8"motto"];
        string user_name = ctx.author.username;
        if (bot.is_emoji)
            user_name = argv[0];
        else
            bot.command->deleteMessage(ctx.channelID, ctx.ID);
        bot.command->sendMessage(ctx.channelID, user_name + ": " + SaoFU::rand_generate(json_vec));
    });

    auto pixiv_data = SaoFU::data_pack("pixiv", u8"送出pixiv的圖片").set_emoji(u8"📦");

    bot.add_command(pixiv_data, [&bot](SleepyDiscord::Message ctx, vector<string> argv) {
        vector<string> file_path = 
            SaoFU::list_dir(bot.get_path[u8"rank_daily"]);
        bot.command->uploadFile(ctx.channelID, SaoFU::rand_generate(file_path), "");
    });

    auto chat_data = SaoFU::data_pack("rrand", u8"組合隨機屁話").set_emoji(u8"❓");

    bot.add_command(chat_data, [&bot, permutations](SleepyDiscord::Message ctx, vector<string> argv) {
        vector<string> vec;
        string str = u8"";
        int size = 0, index = 0;
        for (int i = 0; i < 10; i++) {
            auto tmp = SaoFU::string_split(permutations->origin(), u8"，");
            vec.insert(vec.end(), tmp.begin(), tmp.end());
        }

        do {
            size = SaoFU::rand_generate(1, 4);
        } while (!(size < vec.size()));

        for (int i = 0; i < size; i++) {
            int last_index = index;
            index = SaoFU::rand_generate(vec.size() - 1);
            if (last_index == index) {
                i--;
                continue;
            }
            str += vec[index];
            if (i != size - 1)
                str += u8"，";
        }
       
        bot.command->sendMessage(ctx.channelID, permutations->regroup(str));
    });
}