#pragma once
#include "math/Color.h"
#include "obj/Dir.h"
#include "obj/ObjPtr_p.h"
#include "obj/Object.h"
#include "rndobj/Console.h"
#include "rndobj/Draw.h"
#include "rndobj/Mat.h"
#include "rndobj/Overlay.h"
#include "rndobj/PostProc.h"
#include "rndobj/Tex.h"

class RndCam;
class RndEnviron;
class RndFlare;
class RndLight;

enum Mode {
    kDrawNormal = 0,
    kDrawShadowDepth = 1,
    kDrawExtrude = 2,
    kDrawShadowColor = 3,
    kDrawOcclusion = 4
};

class Rnd : public Hmx::Object, public RndOverlay::Callback {
public:
    class CompressTextureCallback {
    public:
        CompressTextureCallback() {}
        virtual ~CompressTextureCallback() {}
        virtual void TextureCompressed(int) = 0;
    };

    struct PointTest {
        int unk_0x0, unk_0x4, unk_0x8;
        RndFlare *unk_0xC;
    };

    enum Aspect {
        kSquare,
        kRegular,
        kWidescreen,
        kLetterbox
    };

    enum DefaultTextureType {
        kDefaultTex_Black = 0,
        kDefaultTex_White = 1,
        kDefaultTex_WhiteTransparent = 2,
        kDefaultTex_FlatNormal = 3,
        kDefaultTex_Gradient = 4,
        kDefaultTex_Hue = 5,
        kDefaultTex_Error = 6,
        kUnk7 = 7,
        kDefaultTex_Max = 8
    };

    Rnd();
    virtual ~Rnd() {}
    virtual DataNode Handle(DataArray *, bool);
    virtual void PreInit();
    virtual void Init();
    virtual void ReInit() {}
    virtual void Terminate();
    virtual void SetClearColor(const Hmx::Color &c) { mClearColor = c; }
    virtual void ForceColorClear() {}
    virtual void ScreenDump(const char *);
    virtual void ScreenDumpUnique(const char *);
    virtual void
    DrawRect(const Hmx::Rect &, const Hmx::Color &, RndMat *, const Hmx::Color *, const Hmx::Color *) {
    }
    virtual Vector2 &DrawString(const char *, const Vector2 &, const Hmx::Color &, bool);
    virtual void DrawLine(const Vector3 &, const Vector3 &, const Hmx::Color &, bool) {}
    virtual void BeginDrawing();
    virtual void EndDrawing();
    virtual void MakeDrawTarget() {}
    virtual void SetSync(int i) { mSync = i; }
    virtual int GetFrameID() const;
    virtual int GetCurrentFrameTex(bool) { return 0; } // fix return type
    virtual void ReleaseOwnership() {}
    virtual void AcquireOwnership() {}
    virtual void SetShadowMap(RndTex *, RndCam *, const Hmx::Color *) {}
    virtual void SetGSTiming(bool enabled) { mGsTiming = enabled; }
    virtual void CaptureNextGpuFrame() {}
    virtual void RemovePointTest(RndFlare *);
    virtual bool HasDeviceReset() const { return 0; }
    virtual void SetAspect(Aspect a) { mAspect = a; }
    virtual float YRatio();
    virtual int GetShadowMap() { return 0; } // fix return type
    virtual int GetShadowCam() { return 0; } // fix return type
    virtual void SetShrinkToSafeArea(bool b) { mShrinkToSafe = b; }
    virtual bool GetOverscan() const { return 0; }
    virtual void SetOverscan(bool) {}
    virtual void ConfigureRenderMode() {}
    virtual void ChangeYScale(bool) {}
    virtual void SetInGame(bool b) { mInGame = b; }
    virtual int BeginQuery(RndDrawable *) { return -1; }
    virtual int EndQuery(int) { return 0; }
    virtual int VisibleSets(std::vector<RndDrawable *> &, std::vector<RndDrawable *> &) {
        return 0;
    } // fix return type
    virtual void DoWorldBegin();
    virtual void DoWorldEnd();
    virtual void DoPostProcess();
    virtual void DrawPreClear();
    virtual float UpdateOverlay(RndOverlay *, float);
    virtual bool CanModal(bool) { return 0; }
    virtual void ModalDraw(bool, const char *) {}

    bool ShrinkToSafeArea() { return mShrinkToSafe; }
    bool InGame() const { return mInGame; }
    void ShowConsole(bool);
    bool ConsoleShowing();
    void SetShowTimers(bool, bool);
    bool GetEvenOddDisabled() const;
    void SetEvenOddDisabled(bool);
    void UpdateRate();
    void UpdateHeap();
    float DrawTimers(float);
    void Modal(bool &, char *, bool);
    void RegisterPostProcessor(PostProcessor *);
    void UnregisterPostProcessor(PostProcessor *);
    void SetPostProcOverride(RndPostProc *);
    PostProcessor *GetPostProcOverride() const;
    void SetupFont();
    void CreateDefaults();
    void CopyWorldCam(RndCam *);
    void EndWorld();
    void SetProcAndLock(bool);
    bool ProcAndLock() const;
    void
    DrawRectScreen(const Hmx::Rect &, const Hmx::Color &, RndMat *, const Hmx::Color *, const Hmx::Color *);
    void ResetProcCounter();
    void UploadDebugStats();
    RndTex *CreateDefaultTexture(DefaultTextureType);
    Mode DrawMode() const { return mDrawMode; }
    void PreClearDrawAddOrRemove(RndDrawable *, bool, bool);
    bool Unk130() const { return unk130; }
    bool Drawing() const { return mDrawing; }
    RndCam *DefaultCam() { return mDefaultCam; }
    ProcessCmd ProcCmds() const { return mProcCmds; }
    ProcessCmd LastProcCmds() const { return mLastProcCmds; }
    bool TimersShowing() { return mTimersOverlay->Showing(); }
    Vector2 &DrawStringScreen(const char *, const Vector2 &, const Hmx::Color &, bool);
    void CompressTextureCancel(CompressTextureCallback *);
    bool UseVerboseTimers() const { return mVerboseTimers; }
    Mode SetDrawMode(Mode m) {
        Mode old = mDrawMode;
        mDrawMode = m;
        return old;
    }

    DataNode OnShowConsole(const DataArray *);
    DataNode OnToggleTimers(const DataArray *);
    DataNode OnToggleOverlayPosition(const DataArray *);
    DataNode OnToggleTimersVerbose(const DataArray *);
    DataNode OnToggleOverlay(const DataArray *);
    DataNode OnShowOverlay(const DataArray *);
    DataNode OnSetSphereTest(const DataArray *);
    DataNode OnClearColorR(const DataArray *);
    DataNode OnClearColorG(const DataArray *);
    DataNode OnClearColorB(const DataArray *);
    DataNode OnClearColorPacked(const DataArray *);
    DataNode OnSetClearColor(const DataArray *);
    DataNode OnSetClearColorPacked(const DataArray *);
    DataNode OnScreenDump(const DataArray *);
    DataNode OnScreenDumpUnique(const DataArray *);
    DataNode OnScaleObject(const DataArray *);
    DataNode OnReflect(const DataArray *);
    DataNode OnToggleHeap(const DataArray *);
    DataNode OnTestDrawGroups(const DataArray *);
    Aspect GetAspect() const { return mAspect; }
    int Width() const { return mWidth; }
    int Height() const { return mHeight; }
    int ScreenBpp() const { return mScreenBpp; }
    bool DisablePP() const { return mDisablePostProc; }
    RndMat *OverlayMat() const { return mOverlayMat; }
    RndTex *GetNullTexture();

    Hmx::Color mClearColor; // 0x20
    int mWidth; // 0x30
    int mHeight; // 0x34
    int mScreenBpp; // 0x38
    int mDrawCount; // 0x3c
    Timer mDrawTimer; // 0x40
    RndOverlay *mTimersOverlay; // 0x70
    RndOverlay *mRateOverlay; // 0x74
    RndOverlay *mHeapOverlay; // 0x78
    RndOverlay *mStatsOverlay; // 0x7c
    RndConsole *mConsole; // 0x80
    RndMat *mDefaultMat; // 0x84
    RndMat *mOverlayMat; // 0x88
    RndMat *mOverdrawMat; // 0x8c
    RndCam *mDefaultCam; // 0x90
    RndCam *mWorldCamCopy; // 0x94
    RndEnviron *mDefaultEnv; // 0x98
    RndLight *mDefaultLit; // 0x9c

    RndTex *mDefaultTex[7]; // 0xa0 - 0xb8, inclusive

    RndTex *mNullTex; // 0xbc
    float mRateTotal; // 0xc0
    int unkc4; // 0xc4
    int mRateCount; // 0xc8
    unsigned int mFrameID; // 0xcc
    const char *mRateGate; // 0xd0
    DataArray *mFont; // 0xd4
    int mSync; // 0xd8
    bool mGsTiming; // 0xdc
    bool mShowSafeArea; // 0xdd
    bool mDrawing; // 0xde
    bool mWorldEnded; // 0xdf
    Aspect mAspect; // 0xE0
    Mode mDrawMode; // 0xe4 referenced in TexMovie - draw mode enum?
    bool unke8; // 0xe8
    bool unke9; // 0xe9
    bool mShrinkToSafe; // 0xEA
    bool mInGame; // 0xeb
    bool mVerboseTimers; // 0xec
    bool mDisablePostProc; // 0xed
    bool unkee; // 0xee
    bool unkef; // 0xef
    bool unkf0; // 0xf0
    int unkf4; // 0xf4 - funcptr
    int unkf8; // 0xf8
    std::list<PointTest> mPointTests; // 0xfc
    std::list<PostProcessor *> mPostProcessors; // 0x104
    PostProcessor *mPostProcOverride; // 0x10c
    ObjPtrList<RndDrawable> unk110; // 0x110
    ObjPtrList<RndDrawable> mDraws; // 0x120
    bool unk130; // 0x130
    bool unk131; // 0x131
    ProcCounter mProcCounter; // 0x134
    ProcessCmd mProcCmds; // 0x14c
    ProcessCmd mLastProcCmds; // 0x150
    std::list<CompressTextureCallback *> unk154; // 0x154
    int mForceCharLod; // 0x15c
};

extern Rnd *TheRnd;
extern int gSuppressPointTest;
