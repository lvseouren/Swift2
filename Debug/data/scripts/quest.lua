ship = nil
player = nil
Done = false
startDone = false

function Start()
end

function Update()
	if not startDone then
		player = getPlayer()
		ship = getEntity(1)
		
		if ship == nil then
			ship = newEntity()
			add(ship, "Drawable")
			add(ship, "Movable")
			add(ship, "Physical")
			add(ship, "Name")
			
			setTexture(getDrawable(ship), "./data/textures/ship.png")
			setMoveVelocity(getMovable(ship), 100)
			setPosition(getPhysical(ship), 400, 20)
			setSize(getPhysical(ship), getSpriteSize(getDrawable(ship)))
			setName(getName(ship), "Lua Entity")
		end
		
		startDone = true
	end
	
	shipPhys = getPhysical(ship)
	shipPosX, shipPosY = getPosition(shipPhys)
	
	if not Done and isAround(getPhysical(player), shipPosX, shipPosY, 40) then
		print("Congratulations!")
		Done = true
	end
end

function Save()
	print(Done)
	return Done
end

function Load(...)
	Done = ...
	print(Done)
end