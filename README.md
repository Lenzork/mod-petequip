# Pet Equip - Module

[English](README.md) | [Español](README_ES.md)


## How to install

1. Copy the "mod-petequip" folder into your `AzerothCore/modules` folder
2. Execute the SQL File that is in `mod-petequip/sql/characters/base`-folder into your characters Database
2. Rebuild the core

## How to use

1. Go ingame
2. Enter `.petequip {ID}` into the chat. The ID needs to be the number from the column `id` of `creature_equip_template` (which is located in your acore_world database)
3. Then your Pet should have the weapon of the Equipment ID equipped!

## How to add new weapons to equip

1. Go ingame and then summon the Pet that should be able to equip new weapons. (For example: Felguard)
2. Select the Pet and write `.npc info` then you should see a bunch of informations. You want to look for the `current entry: xxx`. Save that entry somewhere
3. Go into your world Database and into the `creature_equip_template` and search for the entry that you've just saved (For example Felguard: 17252)
4. Add a new table entry for the Creature you want to have a weapon. In my case: 17252, next there is the id - if there was an entry before you add +1 to the id. So if there was an entry before and the id was 1 then you do 2 as the id of your new entry. Then on the ItemID1 you enter the item entry of the item you want the pet to have.

## Licensing

The default license of the skeleton-module template is the MIT but you can use a different license for your own modules.

So modules can also be kept private. However, if you need to add new hooks to the core, as well as improving existing ones, you have to share your improvements because the main core is released under the AGPL license. Please [provide a PR](https://www.azerothcore.org/wiki/How-to-create-a-PR) if that is the case.
