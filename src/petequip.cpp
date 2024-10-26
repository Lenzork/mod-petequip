/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "Pet.h"

class PetEquip final : public PetScript
{
public:
    PetEquip() : PetScript("PetEquip") { }

    void OnPetAddToWorld(Pet* pet) override {
        if(sConfigMgr->GetOption<bool>("PetEquip.Enable", false)){ // Check if Module is activated
            ObjectGuid::LowType ownerGUID = pet->GetOwnerGUID().GetCounter();
            if (const QueryResult result = CharacterDatabase.Query("SELECT `equipmentId` FROM `character_pet_equip` WHERE guid = {}", ownerGUID))
            {
                // Receive Equipment Id
                const Field* fields = result->Fetch();
                const int8 equipmentId = fields[0].Get<int8>();

                // Finally execute
                do {
                    pet->SetCurrentEquipmentId(equipmentId);
                    pet->LoadEquipment(equipmentId, true);
                } while (result->NextRow());
            }
        }
    }
};

using namespace Acore::ChatCommands;

class PetEquipCommand final : public CommandScript
{
public:
    PetEquipCommand() : CommandScript("PetEquipCommand") { }

    [[nodiscard]] ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "petequip", HandlePetEquipCommand,    SEC_PLAYER, Console::No }
        };

        return commandTable;
    }

    static bool HandlePetEquipCommand(ChatHandler* handler, int8 args)
    {
        if(sConfigMgr->GetOption<bool>("PetEquip.Enable", false)){ // Check if Module is activated
            if (!args)
            {
                handler->SendSysMessage(LANG_CMD_SYNTAX);
                handler->SetSentErrorMessage(true);
                return false;
            }

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
        return true;
    }
};

// Add all scripts in one
void AddPetEquipScripts()
{
    new PetEquip();
    new PetEquipCommand();
}
