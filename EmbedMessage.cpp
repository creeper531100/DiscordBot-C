#include "EmbedMessage.h"

using std::string;

EmbedMessage::EmbedMessage(string title, string description) {
    SleepyDiscord::Embed::title = title;
    SleepyDiscord::Embed::description = description;
}

void EmbedMessage::set_author(string name, string icon_url) {
    SleepyDiscord::EmbedAuthor embed_author;
    embed_author.name = name;
    embed_author.iconUrl = icon_url;
    SleepyDiscord::Embed::author = embed_author;
}

void EmbedMessage::set_thumbnail(string icon_url) {
    SleepyDiscord::EmbedThumbnail thumbnail;
    thumbnail.url = icon_url;
    SleepyDiscord::Embed::thumbnail = thumbnail;
}

void EmbedMessage::add_field(string name, string value, bool inline_) {
    embed_field.push_back(SleepyDiscord::EmbedField(name, value, inline_));
    SleepyDiscord::Embed::fields = embed_field;
}
