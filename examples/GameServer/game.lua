-- _p, CWorld Global

-- Unit
{_p = nil, CUnit*}

-- Ability
{_p = CAbility*}

-- Unit
Unit = class()
function Unit:ctor(root, name)
end

--[[ internal, create _p for lua obj
function Unit:init(root, name)
end
]]


function onWorldInit()
    -- LoadUnit() internal
    u = Unit:new("Malik", "MK")
    u:setMaxHP(1000)
    u:setHP(100)
    u:setForceByIndex(0);
    u:setPosition()

    a = DefUp:new()
    addTemplateAbility(a)
end



-- TypeValue
TypeValue = class()
function TypeValue:ctor(t, v)
    self.type = t
    self.value = v
end

function TypeValue:setValue(t, v)
    self.type = t
    self.value = v
end

-- AttackValue
AttackValue = class(TypeValue)
AttackValue.kPhysical = 0
AttackValue.kMagical = 1
AttackValue.kSiege = 2
AttackValue.kHoly = 3
function AttackValue:ctor()
    self:sctor(AttackValue.kPhysical, 0)
end

-- ArmorValue
ArmorValue = class(TypeValue)
ArmorValue.kNormal = 0
ArmorValue.kHeavy = 1
ArmorValue.kCrystal = 2
ArmorValue.kWall = 3
ArmorValue.kHero = 4
ArmorValue.kHoly = 5
function ArmorValue:ctor()
    self:sctor(ArmorValue.kNormal, 0)
end

-- AttackBuff
AttackBuff = class()
function AttackBuff:ctor()
end

-- AttackData
AttackData = class()
function AttackData:ctor()
end

function AttackData:setAttackValue(t, v)
    
end


Ability = class()
function Ability:ctor()
end

function Ability:onUnitAddAbility()
end

function Ability:onUnitDelAbility()
end

function Ability:onUnitAbilityReady()
end

function Ability:onUnitRevive()
end

function Ability:onUnitDying()
end

function Ability:onUnitDead()
end

function Ability:onUnitChangeHp(changed)
end

function Ability:onUnitTick(dt)
end

function Ability:onUnitInterval()
end

function Ability:onUnitAttackTarget(attackData, target)
    return ad
end

function Ability:onUnitAttacked(attackData, source)
    return ad
end

function Ability:onUnitDamaged(attackData, source)
end

function Ability:onUnitDamagedDone(damage, source)
end

function Ability:onUnitDamageTargetDone(damage, target)
end

function Ability:onUnitProjectileEffect(projectile)
end

-- ActiveAbility
ActiveAbility = class(Ability)
function ActiveAbility:ctor()
    self:sctor()
end

-- PassiveAbility
PassiveAbility = class(Ability)
function PassiveAbility:ctor()
    self:sctor()
end

-- BuffAbility
BuffAbility = class(Ability)
function BuffAbility:ctor()
    self:sctor()
end

DefAbility = class(PassiveAbility)
function DefAbility:ctor()
    print("a:ctor")
end

function DefAbility:onUnitAddAbility()
    print("onUnitAddAbility")
end

function DefAbility:onUnitDamaged(attackData, source)
    attackData.setAttackValue(kPhysic, 0)
end

a = DefAbility:new()
