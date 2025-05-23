#include "meta_band/Instarank.h"
#include "Utl.h"
#include "game/Defines.h"
#include "os/Debug.h"
#include "ui/UILabel.h"
#include "utl/Locale.h"
#include "utl/Std.h"
#include "utl/Symbols.h"
#include <cstdlib>
#include <cstring>

Instarank::Instarank()
    : mIsValid(0), unk8(0), unkc(0), mScoreType(kScoreBand), mInstaRank(0),
      mIsPercentile(0) {}

void Instarank::Clear() {
    mIsValid = false;
    unk8 = 0;
    unkc = false;
    mInstaRank = 0;
    mIsPercentile = false;
    mStr1 = "";
    mStr2 = "";
}

void Instarank::Init(
    int i1, bool b1, ScoreType ty, int i2, bool b2, String s1, String s2
) {
    mIsValid = true;
    unk8 = i1; // id
    mScoreType = ty;
    unkc = b1; // is_boi
    mInstaRank = i2;
    mIsPercentile = b2;
    mStr1 = s1;
    mStr2 = s2;
}

void Instarank::UpdateRankLabel(UILabel *label) {
    MILO_ASSERT(mIsValid, 0x3A);
    MILO_ASSERT(label, 0x3B);
    if (mIsPercentile) {
        label->SetTokenFmt(instarank_percentile, mInstaRank);
    } else {
        label->SetTokenFmt(instarank_rank, LocalizeSeparatedInt(mInstaRank));
    }
}

bool Instarank::HasHighscore() const {
    MILO_ASSERT(mIsValid, 0x4B);
    char buf[0x100];
    strncpy(buf, mStr1.c_str(), 0xff);
    const char *tok = strtok(buf, "|");
    char tokc = *tok;
    return tokc == 'a' || tokc == 'b';
}

void Instarank::UpdateString1Label(UILabel *label) {
    MILO_ASSERT(mIsValid, 0x58);
    MILO_ASSERT(label, 0x59);
    char buf[0x100];
    strncpy(buf, mStr1.c_str(), sizeof(buf) - 1);
    char tokc = *strtok(buf, "|");
    const char *fontc = GetFontCharFromScoreType(mScoreType, 0);
    switch (tokc) {
    case 'a':
        label->SetTokenFmt(instarank_highscore_percentile, mInstaRank, fontc);
        break;
    case 'b':
        label->SetTokenFmt(
            instarank_highscore_rank, LocalizeSeparatedInt(mInstaRank), fontc
        );
        break;
    case 'c':
        label->SetTokenFmt(
            instarank_previousbest_rank,
            fontc,
            LocalizeSeparatedInt(atoi_s(strtok(0, "|"))),
            LocalizeSeparatedInt(1)
        );
        break;
    case 'd':
        label->SetTokenFmt(
            instarank_rival_percentile,
            LocalizeSeparatedInt(atoi_s(strtok(0, "|"))),
            atoi_s(strtok(0, "|")),
            fontc
        );
        break;
    case 'e':
        label->SetTokenFmt(
            instarank_rival_rank,
            LocalizeSeparatedInt(atoi_s(strtok(0, "|"))),
            LocalizeSeparatedInt(atoi_s(strtok(0, "|"))),
            fontc
        );
        break;
    default:
        MILO_ASSERT(false, 0x8C);
        break;
    }
}

void Instarank::UpdateString2Label(UILabel *label) {
    MILO_ASSERT(mIsValid, 0x94);
    MILO_ASSERT(label, 0x95);
    char buf[0xff];
    strncpy(buf, mStr2.c_str(), sizeof(buf));
    char tokc = *strtok(buf, "|");
    const char *fontc = GetFontCharFromScoreType(mScoreType, 0);
    switch (tokc) {
    case 'f':
        label->SetTextToken(instarank_nofriend_beat);
        break;
    case 'g': {
        const char *tokg = strtok(0, "|");
        int ig = atoi_s(strtok(0, "|"));
        String strg;
        if (ig != 0)
            strg = MakeString(Localize(band_default_name, 0), tokg);
        else
            strg = tokg;
        label->SetTokenFmt(instarank_friend_beat, strg.c_str(), fontc);
    } break;
    case 'h': {
        const char *tokh = strtok(0, "|");
        int ih1 = atoi_s(strtok(0, "|"));
        int ih2 = atoi_s(strtok(0, "|"));
        String strh;
        if (ih2 != 0)
            strh = MakeString(Localize(band_default_name, 0), tokh);
        else
            strh = tokh;
        label->SetTokenFmt(instarank_friend_beat_and_more, fontc, strh.c_str(), ih1);
    } break;
    case 'i': {
        int ii1 = atoi_s(strtok(0, "|"));
        const char *toki = strtok(0, "|");
        int ii2 = atoi_s(strtok(0, "|"));
        String stri;
        if (ii2 != 0)
            stri = MakeString(Localize(band_default_name, 0), toki);
        else
            stri = toki;
        label->SetTokenFmt(
            instarank_rival_close, LocalizeSeparatedInt(ii1), stri.c_str(), fontc
        );
    } break;
    case 'j':
        label->SetTextToken(instarank_nofriend_beat);
        break;
    default:
        MILO_ASSERT(false, 0xEB);
        break;
    }
}