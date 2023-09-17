# Tutorials - Mac Caveats

There are issues some users might run into on macOS when using Godot and GodotSteam. This tutorial page is here to cover them.

---

## M1 and M2 Machine Issues

From user **canardbleu**, a handy tip about using the pre-compiled editors and macOS M1 and M2 machines.

Their team said "[we were] unable to launch any scene because of the 'files & folders' permission popup that keeps popping and popping. Fortunately, we were able to find a workaround by executing the following command."

`codesign -s - --deep /Applications/GodotEditor.app`
