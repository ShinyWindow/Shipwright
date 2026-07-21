#include "SohMenu.h"

namespace SohGui {

extern std::shared_ptr<SohMenu> mSohMenu;
using namespace UIWidgets;

static const std::unordered_map<int32_t, const char*> vrMirrorAxisOptions = {
    { 0, "X (finger axis)" },
    { 1, "Y" },
    { 2, "Z (thumb axis)" },
};

// Must match the value encoding read by padmgr.c's VR button mapping.
static const std::unordered_map<int32_t, const char*> vrN64ButtonOptions = {
    { 0, "None" },     { 1, "A" },        { 2, "B (Sword)" }, { 3, "Z (Target)" },
    { 4, "R (Shield)" }, { 5, "L" },      { 6, "Start (Pause)" }, { 7, "C-Up" },
    { 8, "C-Down" },   { 9, "C-Left" },   { 10, "C-Right" },
};

void SohMenu::AddMenuVRSettings() {
    AddMenuEntry("VR Settings", CVAR_SETTING("Menu.VRSettingsSidebarSection"));

    AddSidebarEntry("VR Settings", "Movement", 1);
    WidgetPath movementPath = { "VR Settings", "Movement", SECTION_COLUMN_1 };

    AddWidget(movementPath, "Turning", WIDGET_SEPARATOR_TEXT);
    AddWidget(movementPath, "Snap Turning (Right Stick)", WIDGET_CVAR_CHECKBOX)
        .CVar("gVrSnapTurnOn")
        .Options(CheckboxOptions()
                     .DefaultValue(true)
                     .Tooltip("Flick the right thumbstick left/right to rotate the world in discrete "
                              "steps. While enabled, the stick's X axis no longer triggers C-Left/"
                              "C-Right (C-Up/C-Down on the Y axis still work)."));
    AddWidget(movementPath, "Snap Turn Angle: %.0f deg", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrSnapTurnDegrees")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gVrSnapTurnOn", 1); })
        .Options(FloatSliderOptions()
                     .Min(10.0f)
                     .Max(180.0f)
                     .DefaultValue(45.0f)
                     .Step(5.0f)
                     .Format("%.0f")
                     .Tooltip("Degrees rotated per flick of the stick."));

    AddSidebarEntry("VR Settings", "Buttons", 2);
    WidgetPath buttonsPath = { "VR Settings", "Buttons", SECTION_COLUMN_1 };

    AddWidget(buttonsPath, "Left Controller", WIDGET_SEPARATOR_TEXT);
    AddWidget(buttonsPath, "Left Trigger", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnLTrigger")
        .Options(ComboboxOptions().DefaultIndex(3).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "Left Grip", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnLGrip")
        .Options(ComboboxOptions().DefaultIndex(4).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "X Button", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnLPrimary")
        .Options(ComboboxOptions().DefaultIndex(9).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "Y Button", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnLSecondary")
        .Options(ComboboxOptions().DefaultIndex(10).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "Left Stick Click", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnLStickClick")
        .Options(ComboboxOptions().DefaultIndex(0).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "Left Menu Button", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnLMenu")
        .Options(ComboboxOptions()
                     .DefaultIndex(6)
                     .ComboMap(vrN64ButtonOptions)
                     .Tooltip("Make sure SOMETHING maps to Start, or you won't be able to pause."));

    buttonsPath.column = SECTION_COLUMN_2;
    AddWidget(buttonsPath, "Right Controller", WIDGET_SEPARATOR_TEXT);
    AddWidget(buttonsPath, "Right Trigger", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnRTrigger")
        .Options(ComboboxOptions().DefaultIndex(2).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "Right Grip", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnRGrip")
        .Options(ComboboxOptions().DefaultIndex(0).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "A Button", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnRPrimary")
        .Options(ComboboxOptions().DefaultIndex(1).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "B Button", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnRSecondary")
        .Options(ComboboxOptions().DefaultIndex(8).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "Right Stick Click", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnRStickClick")
        .Options(ComboboxOptions().DefaultIndex(0).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "Right Menu Button", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrBtnRMenu")
        .Options(ComboboxOptions().DefaultIndex(0).ComboMap(vrN64ButtonOptions));
    AddWidget(buttonsPath, "Sticks are fixed: left = movement, right = C-buttons/snap turn.", WIDGET_TEXT);

    AddSidebarEntry("VR Settings", "Camera", 1);
    WidgetPath cameraPath = { "VR Settings", "Camera", SECTION_COLUMN_1 };

    AddWidget(cameraPath, "Head Position (relative to Link's body)", WIDGET_SEPARATOR_TEXT);
    AddWidget(cameraPath, "Height Offset: %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHeadHeightOffset")
        .Options(FloatSliderOptions()
                     .Min(-100.0f)
                     .Max(100.0f)
                     .DefaultValue(-9.0f)
                     .Step(1.0f)
                     .Format("%.1f")
                     .Tooltip("Raise/lower the eye anchor relative to Link's eye height, in game "
                              "units. Lowering it brings the ground closer by exactly offset / world "
                              "scale meters."));
    AddWidget(cameraPath, "Forward Offset: %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHeadOffsetForward")
        .Options(FloatSliderOptions()
                     .Min(-60.0f)
                     .Max(60.0f)
                     .DefaultValue(6.0f)
                     .Step(1.0f)
                     .Format("%.1f")
                     .Tooltip("Move the eye anchor along Link's facing (game units). Positive pushes "
                              "the camera forward out of his head; negative pulls it back."));
    AddWidget(cameraPath, "Side Offset: %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHeadOffsetSide")
        .Options(FloatSliderOptions()
                     .Min(-60.0f)
                     .Max(60.0f)
                     .DefaultValue(0.0f)
                     .Step(1.0f)
                     .Format("%.1f")
                     .Tooltip("Move the eye anchor sideways relative to Link's facing (game units)."));
    AddWidget(cameraPath, "World Scale", WIDGET_SEPARATOR_TEXT);
    AddWidget(cameraPath, "World Scale: %.1f units/m", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrWorldScale")
        .Options(FloatSliderOptions()
                     .Min(10.0f)
                     .Max(100.0f)
                     .DefaultValue(35.0f)
                     .Step(0.5f)
                     .Format("%.1f")
                     .Tooltip("Game units per real-world meter. Higher makes the world feel smaller "
                              "(and physical movements cover more in-game distance); lower makes "
                              "everything tower over you. Applies live."));
    AddWidget(cameraPath, "Menu Screen", WIDGET_SEPARATOR_TEXT);
    AddWidget(cameraPath, "Menu Screen Distance: %.1f m", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrScreenDistance")
        .Options(FloatSliderOptions()
                     .Min(0.5f)
                     .Max(5.0f)
                     .DefaultValue(2.2f)
                     .Step(0.1f)
                     .Format("%.1f")
                     .Tooltip("How far in front of you the floating menu panel (file select, pause) "
                              "appears. Applies the next time a menu opens."));
    AddWidget(cameraPath, "Menu Screen Size: %.1f m", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrScreenSize")
        .Options(FloatSliderOptions()
                     .Min(0.5f)
                     .Max(5.0f)
                     .DefaultValue(2.4f)
                     .Step(0.1f)
                     .Format("%.1f")
                     .Tooltip("Width of the floating menu panel in meters (height follows 4:3). "
                              "Applies live."));
    AddWidget(cameraPath, "Export", WIDGET_SEPARATOR_TEXT);
    AddWidget(cameraPath, "Copy Camera Values to Clipboard", WIDGET_BUTTON)
        .Options(ButtonOptions().Tooltip("Copy the head-position and world-scale values so they can "
                                         "be handed to a developer to become the defaults."))
        .Callback([](WidgetInfo& info) {
            char buf[256];
            snprintf(buf, sizeof(buf),
                     "gVrHeadHeightOffset=%.1f\n"
                     "gVrHeadOffsetForward=%.1f\n"
                     "gVrHeadOffsetSide=%.1f\n"
                     "gVrWorldScale=%.1f\n"
                     "gVrScreenDistance=%.1f\n"
                     "gVrScreenSize=%.1f\n",
                     CVarGetFloat("gVrHeadHeightOffset", -9.0f), CVarGetFloat("gVrHeadOffsetForward", 6.0f),
                     CVarGetFloat("gVrHeadOffsetSide", 0.0f), CVarGetFloat("gVrWorldScale", 35.0f),
                     CVarGetFloat("gVrScreenDistance", 2.2f), CVarGetFloat("gVrScreenSize", 2.4f));
            ImGui::SetClipboardText(buf);
        });

    AddSidebarEntry("VR Settings", "Hands", 3);
    WidgetPath path = { "VR Settings", "Hands", SECTION_COLUMN_1 };

    AddWidget(path, "General", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Motion-Control Hands", WIDGET_CVAR_CHECKBOX)
        .CVar("gVrMotionHands")
        .Options(CheckboxOptions()
                     .DefaultValue(true)
                     .Tooltip("Detach Link's hands from his body and pin them to the VR controllers."));
    AddWidget(path, "Left-Handed Mode", WIDGET_CVAR_CHECKBOX)
        .CVar("gVrLeftHanded")
        .Options(CheckboxOptions().Tooltip(
            "Swap which controller drives the sword hand: the LEFT controller holds the sword (matching "
            "Link's own left-handedness) instead of the right."));
    AddWidget(path, "Mirror Sword Hand", WIDGET_CVAR_CHECKBOX)
        .CVar("gVrHandMirrorSword")
        .Options(CheckboxOptions()
                     .DefaultValue(true)
                     .Tooltip("Reflect the sword hand's mesh so it reads as a right hand on the right "
                              "controller. Only applies in right-handed mode."));
    AddWidget(path, "Mirror Shield Hand", WIDGET_CVAR_CHECKBOX)
        .CVar("gVrHandMirrorShield")
        .Options(CheckboxOptions()
                     .DefaultValue(true)
                     .Tooltip("Reflect the shield hand's mesh so it reads as a left hand on the left "
                              "controller. Note the reflection also mirrors the shield's face design; "
                              "pair with the Left Hand Override values to orient it correctly."));
    AddWidget(path, "Mirror Axis", WIDGET_CVAR_COMBOBOX)
        .CVar("gVrHandMirrorAxis")
        .Options(ComboboxOptions()
                     .DefaultIndex(2)
                     .ComboMap(vrMirrorAxisOptions)
                     .Tooltip("Which model-local axis the mirror reflection negates. Should be the thumb "
                              "axis: it must keep the finger direction and flip the thumb so the mesh reads "
                              "as the opposite hand. Try each if the hands look inside-out."));

    path.column = SECTION_COLUMN_2;
    AddWidget(path, "Hand Rotation", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Tune while looking at the SWORD hand - the other hand mirrors automatically.", WIDGET_TEXT);
    AddWidget(path, "Pitch: %.1f deg", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandCalPitch")
        .Options(FloatSliderOptions()
                     .Min(-180.0f)
                     .Max(180.0f)
                     .DefaultValue(88.0f)
                     .Step(1.0f)
                     .Format("%.1f")
                     .Tooltip("Rotation about the grip X axis (wrist tilt up/down)."));
    AddWidget(path, "Yaw: %.1f deg", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandCalYaw")
        .Options(FloatSliderOptions()
                     .Min(-180.0f)
                     .Max(180.0f)
                     .DefaultValue(-100.0f)
                     .Step(1.0f)
                     .Format("%.1f")
                     .Tooltip("Rotation about the grip Y axis. If the sword points backward or sideways "
                              "out of your fist, adjust this first."));
    AddWidget(path, "Roll: %.1f deg", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandCalRoll")
        .Options(FloatSliderOptions()
                     .Min(-180.0f)
                     .Max(180.0f)
                     .DefaultValue(80.0f)
                     .Step(1.0f)
                     .Format("%.1f")
                     .Tooltip("Rotation about the grip Z axis (twist around the handle - use to line up "
                              "the blade edge and palm)."));

    AddWidget(path, "Left Hand Override", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Tune Left Hand Separately", WIDGET_CVAR_CHECKBOX)
        .CVar("gVrHandLOverride")
        .Options(CheckboxOptions().DefaultValue(true).Tooltip(
            "By default the left controller's hand is derived from the values above by mirror symmetry. "
            "If it doesn't look right, enable this and dial it in with its own values below."));
    AddWidget(path, "L Pitch: %.1f deg", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandLCalPitch")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gVrHandLOverride", 1); })
        .Options(FloatSliderOptions().Min(-180.0f).Max(180.0f).DefaultValue(-149.0f).Step(1.0f).Format("%.1f"));
    AddWidget(path, "L Yaw: %.1f deg", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandLCalYaw")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gVrHandLOverride", 1); })
        .Options(FloatSliderOptions().Min(-180.0f).Max(180.0f).DefaultValue(76.0f).Step(1.0f).Format("%.1f"));
    AddWidget(path, "L Roll: %.1f deg", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandLCalRoll")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gVrHandLOverride", 1); })
        .Options(FloatSliderOptions().Min(-180.0f).Max(180.0f).DefaultValue(30.0f).Step(1.0f).Format("%.1f"));
    AddWidget(path, "L Offset X: %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandLOffX")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gVrHandLOverride", 1); })
        .Options(FloatSliderOptions().Min(-30.0f).Max(30.0f).DefaultValue(0.0f).Step(0.5f).Format("%.1f"));
    AddWidget(path, "L Offset Y: %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandLOffY")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gVrHandLOverride", 1); })
        .Options(FloatSliderOptions().Min(-30.0f).Max(30.0f).DefaultValue(0.0f).Step(0.5f).Format("%.1f"));
    AddWidget(path, "L Offset Z: %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandLOffZ")
        .PreFunc([](WidgetInfo& info) { info.isHidden = !CVarGetInteger("gVrHandLOverride", 1); })
        .Options(FloatSliderOptions().Min(-30.0f).Max(30.0f).DefaultValue(0.0f).Step(0.5f).Format("%.1f"));

    path.column = SECTION_COLUMN_3;
    AddWidget(path, "Hand Position", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Offset X: %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandOffX")
        .Options(FloatSliderOptions()
                     .Min(-30.0f)
                     .Max(30.0f)
                     .DefaultValue(0.0f)
                     .Step(0.5f)
                     .Format("%.1f")
                     .Tooltip("Slide the hand along the grip X axis (game units, tuned for the left "
                              "controller; the right controller mirrors)."));
    AddWidget(path, "Offset Y: %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandOffY")
        .Options(FloatSliderOptions()
                     .Min(-30.0f)
                     .Max(30.0f)
                     .DefaultValue(0.0f)
                     .Step(0.5f)
                     .Format("%.1f")
                     .Tooltip("Slide the hand along the grip Y axis."));
    AddWidget(path, "Offset Z: %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gVrHandOffZ")
        .Options(FloatSliderOptions()
                     .Min(-30.0f)
                     .Max(30.0f)
                     .DefaultValue(0.0f)
                     .Step(0.5f)
                     .Format("%.1f")
                     .Tooltip("Slide the hand along the grip Z axis (roughly along the handle)."));

    AddWidget(path, "Export", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Copy Values to Clipboard", WIDGET_BUTTON)
        .Options(ButtonOptions().Tooltip("Copy every VR hand-tuning value to the clipboard so it can be "
                                         "pasted into a bug report or handed to a developer."))
        .Callback([](WidgetInfo& info) {
            char buf[1024];
            snprintf(buf, sizeof(buf),
                     "gVrMotionHands=%d\n"
                     "gVrLeftHanded=%d\n"
                     "gVrHandMirrorSword=%d\n"
                     "gVrHandMirrorShield=%d\n"
                     "gVrHandMirrorAxis=%d\n"
                     "gVrHandCalPitch=%.1f\n"
                     "gVrHandCalYaw=%.1f\n"
                     "gVrHandCalRoll=%.1f\n"
                     "gVrHandOffX=%.1f\n"
                     "gVrHandOffY=%.1f\n"
                     "gVrHandOffZ=%.1f\n"
                     "gVrHandLOverride=%d\n"
                     "gVrHandLCalPitch=%.1f\n"
                     "gVrHandLCalYaw=%.1f\n"
                     "gVrHandLCalRoll=%.1f\n"
                     "gVrHandLOffX=%.1f\n"
                     "gVrHandLOffY=%.1f\n"
                     "gVrHandLOffZ=%.1f\n",
                     CVarGetInteger("gVrMotionHands", 1), CVarGetInteger("gVrLeftHanded", 0),
                     CVarGetInteger("gVrHandMirrorSword", 1), CVarGetInteger("gVrHandMirrorShield", 1),
                     CVarGetInteger("gVrHandMirrorAxis", 2),
                     CVarGetFloat("gVrHandCalPitch", 88.0f), CVarGetFloat("gVrHandCalYaw", -100.0f),
                     CVarGetFloat("gVrHandCalRoll", 80.0f), CVarGetFloat("gVrHandOffX", 0.0f),
                     CVarGetFloat("gVrHandOffY", 0.0f), CVarGetFloat("gVrHandOffZ", 0.0f),
                     CVarGetInteger("gVrHandLOverride", 1), CVarGetFloat("gVrHandLCalPitch", -149.0f),
                     CVarGetFloat("gVrHandLCalYaw", 76.0f), CVarGetFloat("gVrHandLCalRoll", 30.0f),
                     CVarGetFloat("gVrHandLOffX", 0.0f), CVarGetFloat("gVrHandLOffY", 0.0f),
                     CVarGetFloat("gVrHandLOffZ", 0.0f));
            ImGui::SetClipboardText(buf);
        });
}

} // namespace SohGui
