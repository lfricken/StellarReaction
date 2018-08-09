Project Started 2014-02-13<br />
Readme Updated: 2016-12-01<br />

# Build TGUI
You need release libraries directly under lib in SFML root.

# StellarReaction
Stellar Reaction is a real-time, multiplayer, space combat video game. Gameplay consists of four teams of Human or AI players controlling space ships, which are used to attack enemy ships, and to capture objective points to earn money.

# [Gameplay Mechanics](https://github.com/Strikerklm96/StellarReaction/wiki/Gameplay-Mechanics)

# Generate Documentation
1. Install Doxygen.
2. Set working directory to StellarReaction root directory.
3. Run command: doxygen doc/doxygen.cfg

# Contributors
Leon Frickensmith<br />
Evan Defend<br />
Dylan Schlaht<br />
Henry Lee<br />
Shaan Iqbal<br />
Yuto Otaguro<br />
Alexander McArthur<br />
Andrew Borg<br />
Sahil Suhag<br />
Michael Wojciak<br />
<br />



# Gameplay Details

## Modular Ships
Ships consist entirely of modules which provide all the functionality to a ship. More modules can be bought at a store for some amount of money, but there is a limit to the number of modules that can be put on a ship, and on where they can be placed. Individual modules control all of the following: Camera Zooming, Cloaking, Firing Weapons, Energy Production, Ship Propulsion, and Teleportation.

## Combat
Combat features ships firing at one another, and using their environment to their advantage. Damage is dealt to a ship by reducing the health of the ships modules. When modules have less than 25% health, they no longer provide whatever functionality they used to. In addition, there are hazards around the different maps that can be used to one’s advantage. A player can control which weapons fire at a given time by toggling their weapon control groups to on or off.

## Hazards
Black Holes pull objects toward it that come too close. When an object approaches the center of the Black Hole, damage is dealt to the object, potentially destroying a ship. Ships can escape a Black Hole with the right technique and tools however. Asteroids are another hazard that players should be wary of. They have a large amount of mass, and will damage a player's ship if they run into it. Dense asteroid fields can provide a route of escape for a losing ship, or graveyard for careless pilots.

## Weapons
There are four classes of weapons available to the player. Lasers, Projectiles, Missiles, and Grappling Hooks. Lasers consume lots of energy, but have the advantage of being good at medium range combat due to their time of flight being nearly instant. They are good for damaging specific modules of the enemy ship. Projectile weapons can be tricky to aim, but deal large amounts of damage, making them ideal for close range combat. Missiles are designed for long range combat. They target the closest object to the player's cursor when fired. They will track the target until they hit it or run out of fuel and explode. A player can used quick turning and obstacles to avoid a Missile. Grappling Hooks pull the target ship towards the player's ship, enabling a player to catch a fleeing opponent.

## CMake
*SFML - Point at directory. Pretty easy.
*TGUI - Build SFML, put built libs into TGUI/lib (note: they cannot be in sub folders). Point at directory.
