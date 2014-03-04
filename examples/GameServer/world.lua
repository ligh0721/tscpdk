function onWorldInit()
    s = Sprite:new("Malik")
    s:prepareFrame(Sprite.kFrmDefault, "default")
    s:prepareAnimation(Sprite.kAniMove, "move", -1)
    s:prepareAnimation(Sprite.kAniDie, "die", -1)
    s:prepareAnimation(Sprite.kAniAct1, "act1", 4)
    s:prepareAnimation(Sprite.kAniAct2, "act2", 4)
    s:setGeometry(7.0, 10.0, 0.5, 0.1125, 10.0, 10.0)
    u = Unit:new(s, "MK")
	
    u:setForceByIndex(1)
	u:setMaxHp(1375.0)
    u:setBaseArmor(0, 9.0)
    u:setBaseMoveSpeed(80.0)
    u:setPosition(100, 100)
	

	setControlUnit(u)

	a = StatusShowPas:new()
	u:addPassiveAbility(a)

	a = AttackAct:new("Attack", "NormalAttack", 1.5, 0, 10, 0.15,   -3, 50, true, Sprite.kAniAct1, Sprite.kAniAct2)
	u:addActiveAbility(a)
	
end
