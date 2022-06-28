/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "Pet.h"

// Pet Equipment Module by Lenzork
// Have fun with it!
class PetEquip : public PetScript
{
public:
    PetEquip() : PetScript("PetEquip") { }

    void OnPetAddToWorld(Pet* pet) {
        if(sConfigMgr->GetOption<bool>("PetEquip.Enable", false)){ // Check if Module is activated
            int ownerGUID = pet->GetOwnerGUID().GetCounter();
            QueryResult result = CharacterDatabase.Query("SELECT `equipmentId` FROM `character_pet_equip` WHERE guid = {}", ownerGUID);
            if (result)
            {
                // Finally execute
                do {
                    pet->SetCurrentEquipmentId((*result)[0].Get<int>());
                    pet->LoadEquipment((*result)[0].Get<int>(), true);
                } while (result->NextRow());
            }
        }
    }
};

class PetEquipCommand : public CommandScript
{
public:
    PetEquipCommand() : CommandScript("PetEquipCommand") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable =
        {
            { "petequip",   SEC_PLAYER,      false,            HandlePetEquipCommand,         "Enter a number after it to change the Player Pets equipment Table if there are some. Helpful for example for Warlocks!"}
        };

        return commandTable;
    }

    static bool HandlePetEquipCommand(ChatHandler* handler, int args)
    {
        if(sConfigMgr->GetOption<bool>("PetEquip.Enable", false)){ // Check if Module is activated
            if (!args)
            {
                handler->SendSysMessage(LANG_CMD_SYNTAX);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (args) {
                if(handler->GetPlayer()->GetPet()){
                    Pet* playerPet = handler->GetPlayer()->GetPet();
                    uint32 ownerGUID = handler->GetPlayer()->GetGUID().GetCounter();
                    CharacterDatabase.Execute("INSERT IGNORE INTO `character_pet_equip` (guid, equipmentId) VALUES ({}, 1)", ownerGUID);
                    playerPet->SetCurrentEquipmentId(args);
                    CharacterDatabase.Execute("UPDATE character_pet_equip SET equipmentId = {} WHERE guid={}", args, ownerGUID);
                    playerPet->LoadEquipment(args, true);
                    playerPet->Say("A new Weapon? Hmpf...", LANG_UNIVERSAL);
                    handler->PSendSysMessage("Successfully set the Pet Equipment ID to " + args);
                    handler->PSendSysMessage(args);
                }
                else {
                    handler->PSendSysMessage("You do not have a Pet!");
                }
            }
        }
        return true;
    }
};

// Add all scripts in one
void AddPetEquipScripts()
{
    new PetEquip();
    new PetEquipCommand();
}
