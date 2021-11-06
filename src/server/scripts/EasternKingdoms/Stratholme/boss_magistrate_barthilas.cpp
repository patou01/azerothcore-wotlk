/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameObjectAI.h"
#include "MoveSplineInit.h"
#include "Player.h"
#include "stratholme.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"

enum Spells
{

};

enum Events
{
    INTRO_1                           = 1,
    INTRO_2                           = 2,
    INTRO_3                           = 3,
    INTRO_4                           = 4,
    INTRO_5                           = 5,
    INTRO_6                           = 6,
};

enum Misc
{
    SAY_DOOR_OPEN               = 0,
    DATA_DOOR_OPEN              = 1,
    POINT_MAGISTRATE_DESPAWN    = 1,
    PATH_MAGISTRATE_FLEE        = 1,
};

Position const PosMove[2] =
{
    { 299.4884f, 92.76137f, 105.6335f, 0.0f },
    { 314.8673f, 90.30210f, 101.6459f, 0.0f }
};

class boss_magistrate_barthilas : public CreatureScript
{
public:
    boss_magistrate_barthilas() : CreatureScript("boss_magistrate_barthilas") {}

    struct boss_magistrate_barthilasAI : public BossAI
    {
        boss_magistrate_barthilasAI(Creature* creature) : BossAI(creature, DATA_MAGISTRATE_BARTHILAS)
        {
        }

        EventMap events;

        void EnterCombat(Unit* /*who*/) override
        {
            events.Reset();
        }

        void JustDied(Unit* /*killer*/) override
        {
            instance->SetData(DATA_MAGISTRATE_BARTHILAS, DONE);
        }
        
        void MovementInform(uint32 type, uint32 id) override
        {
            LOG_FATAL("Entities:unit", "moving type %d, id %d", type, id);
            if (id == POINT_MAGISTRATE_DESPAWN)
            {
                LOG_FATAL("Entities:Unit", "done pathing");
                me->DespawnOrUnsummon();
            }
        }

        void SetData(uint32 type, uint32 data) override
        {
            if (type == 1)
            {
                if (data == 1)
                {
                    me->GetMotionMaster()->MovePath(PATH_MAGISTRATE_FLEE, false);
                   // me->SetSpeed(MOVE_RUN, 14.0f, true);
                    Talk(SAY_DOOR_OPEN);
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
            {
                return;
            }

            events.Update(diff);
            if (me->HasUnitState(UNIT_STATE_CASTING))
            {
                return;
            }

            switch (events.ExecuteEvent())
            {
               
            }

            DoZoneInCombat();
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetStratholmeAI<boss_magistrate_barthilasAI>(creature);
    }
};

void AddSC_boss_magistrate_barthilas()
{
    new boss_magistrate_barthilas();
}
