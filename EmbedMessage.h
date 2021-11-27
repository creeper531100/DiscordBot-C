#ifndef _EMBED_MESSAGE
#define _EMBED_MESSAGE
#include "sleepy_discord/sleepy_discord.h"

class EmbedMessage : public SleepyDiscord::Embed {
    std::vector<SleepyDiscord::EmbedField> embed_field;
public:
    EmbedMessage(std::string, std::string);
    void set_author(std::string, std::string);
    void set_thumbnail(std::string);
    void add_field(std::string, std::string, bool inline_ = 0);
};

#endif
