# Godot Steam Multiplayer

This is a tool for pulling GodotSteam into Godot's HLMP. It allows you to use Godot's RPC and `MultiplayerSynchronizer`/`MultiplayerSpawner` nodes. It is dependant on GodotSteam.

# Compiling

First, follow [the instructions here](https://godotsteam.com/howto/modules/) for compiling Godot with GodotSteam.

Then, in your Godot's `modules` folder, clone this branch of Godot Steam directly to a new `godotsteammultiplayer` directory.

```bash
cd modules
git clone git@github.com:CoaguCo-Industries/GodotSteam.git -b "multiplayer/alpha" godotsteammultiplayer
```

Compile Godot as normal, which will now include this SteamMultiplayerPeer.
