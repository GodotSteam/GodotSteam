# Tutorials - Common Issues</h1>

In this tutorial, we will cover some common pitfalls people find themselves in.  This should help you troubleshoot some of the basics for when things don't seem to be working.

---

## steam_appid.txt.txt

For Windows users, when creating the steam_appid.txt file you need to watch out for the dreaded .txt.txt extension.  If you have extensions hidden in the file explorer, this is more likely to happen since you can't see that extra .txt.  If you do have them hidden, make sure to skip added .txt to the end.

---

## Error 79 When Initializing

This can have quite a few causes but a common one is not having your depots or packages set up correctly.  Check out this [page in the official Steamworks SDK documentation for more on how to do it.](https://partner.steamgames.com/doc/store/application/packages){ target="_blank" }

------

## Achievements Not Working

Sometimes your brand new achievements don't seem to be triggering.  One cause can be that you didn't publish them in the Steamworks back-end.  Once they are added into Steam's system, you'll need to publish the changes to be able to work with them.

---

## Leaderboard Names

I haven't actually confirmed this yet, but it seems that a dash in the leaderboard name will cause it to fail.  For example, **this-leaderboard** will probably not work but **this_leaderboard** will.

Also remember that leaderboards need to be published to be functional.

---

## Using The Module And Plug-in

On occasion folks will download the pre-compiled editor (module version) and then install the plug-in from the Godot Asset Library.  This will result in a few weird errors, most of which will prevent anything from working.  If you do this, you'll need to pick one or the other.

If you stick with the pre-compiles, which is recommended, you'll need to completely remove the plug-in to get things working again.