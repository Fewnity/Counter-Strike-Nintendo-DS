#include "main.h"
#include "collisions.h"
#include "network.h"
#include "ai.h"
#include "sounds.h"
#include "movements.h"
#include "party.h"

Stairs *AllStairsRef;
Wall *AllWallsCollisionsRef;
Player *AllPlayersRefForCollisions;
PhysicalGrenade *grenades[GrenadeCount];

void SetPlayerForCollisions()
{
    AllPlayersRefForCollisions = GetPlayers();
}

void CalculateAllTriggerColBoxs()
{
    // Add occlusion culling triggers
    CalculateTriggerColBox(-9.52357, 57.34258, 15.70023, 20.76961, 0);
    CalculateTriggerColBox(-32.8, 31.70565, 25.39098, 32.87455, 1);
    CalculateTriggerColBox(-30.03613, 9.913523, 7.458398, 9.875763, 2);
    CalculateTriggerColBox(-12.07624, 8.24, 20.75247, 13, 3);
    CalculateTriggerColBox(-38.87369, 3.970799, 9.822498, 15.72273, 4);
    CalculateTriggerColBox(-31.71968, 57.28525, 27.55163, 17.77374, 5);
    CalculateTriggerColBox(10.59276, 58.97986, 23.88577, 17.49506, 6);
    CalculateTriggerColBox(22.83467, 41.8413, 16.72676, 16.5031, 7);
    CalculateTriggerColBox(27.63166, 29.56198, 6.307827, 7.687183, 8);
    CalculateTriggerColBox(29.65833, 17.5136, 11.42418, 16.10892, 9);
    CalculateTriggerColBox(47.04734, 20.07397, 22.30289, 42.11788, 10);
    CalculateTriggerColBox(31.25437, -19.58616, 52.12261, 37.01623, 11);
    CalculateTriggerColBox(11.91049, 2.419476, 25.90689, 6.575387, 12);
    CalculateTriggerColBox(-13.12243, -19.91082, 15.07712, 12.67715, 13);
    CalculateTriggerColBox(-11.61234, -19.34598, 18.0973, 11.54748, 14);
    CalculateTriggerColBox(-7.77587, -16.08564, 25.74453, 30.01518, 15);
    CalculateTriggerColBox(-31.87811, -8.83736, 22.03724, 15.51862, 16);
    CalculateTriggerColBox(-31.87811, -28.16135, 22.03724, 23.0819, 17);
    CalculateTriggerColBox(-24.4092, 11.52498, 3.830673, 6.652855, 18);
    CalculateTriggerColBox(0.2512362, 31.8348, 16.78329, 36.75443, 19);
    CalculateTriggerColBox(3.534717, 9.606822, 9.155338, 8.084706, 20);

    // All occlusion culling zones data (a zone will show one or multiple zones)
    int currentZone = 0;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 0;
    AllZones[currentZone].AllVisibleZones[1] = 4;
    AllZones[currentZone].AllVisibleZones[2] = 6;
    AllZones[currentZone].id = currentZone;

    currentZone = 1;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 0;
    AllZones[currentZone].AllVisibleZones[1] = 1;
    AllZones[currentZone].AllVisibleZones[2] = 4;
    AllZones[currentZone].id = currentZone;

    currentZone = 2;
    AllZones[currentZone].ZoneCount = 2;
    AllZones[currentZone].AllVisibleZones[0] = 0;
    AllZones[currentZone].AllVisibleZones[1] = 1;
    AllZones[currentZone].id = currentZone;

    currentZone = 3;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 1;
    AllZones[currentZone].AllVisibleZones[1] = 3;
    AllZones[currentZone].AllVisibleZones[2] = 4;
    AllZones[currentZone].id = currentZone;

    currentZone = 4;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 0;
    AllZones[currentZone].AllVisibleZones[1] = 1;
    AllZones[currentZone].AllVisibleZones[2] = 2;
    AllZones[currentZone].id = currentZone;

    currentZone = 5;
    AllZones[currentZone].ZoneCount = 4;
    AllZones[currentZone].AllVisibleZones[0] = 0;
    AllZones[currentZone].AllVisibleZones[1] = 1;
    AllZones[currentZone].AllVisibleZones[2] = 4;
    AllZones[currentZone].AllVisibleZones[3] = 6;
    AllZones[currentZone].id = currentZone;

    currentZone = 6;
    AllZones[currentZone].ZoneCount = 4;
    AllZones[currentZone].AllVisibleZones[0] = 0;
    AllZones[currentZone].AllVisibleZones[1] = 3;
    AllZones[currentZone].AllVisibleZones[2] = 4;
    AllZones[currentZone].AllVisibleZones[3] = 6;
    AllZones[currentZone].id = currentZone;

    currentZone = 7;
    AllZones[currentZone].ZoneCount = 2;
    AllZones[currentZone].AllVisibleZones[0] = 4;
    AllZones[currentZone].AllVisibleZones[1] = 6;
    AllZones[currentZone].id = currentZone;

    currentZone = 8;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 4;
    AllZones[currentZone].AllVisibleZones[1] = 5;
    AllZones[currentZone].AllVisibleZones[2] = 6;
    AllZones[currentZone].id = currentZone;

    currentZone = 9;
    AllZones[currentZone].ZoneCount = 2;
    AllZones[currentZone].AllVisibleZones[0] = 5;
    AllZones[currentZone].AllVisibleZones[1] = 6;
    AllZones[currentZone].id = currentZone;

    currentZone = 10;
    AllZones[currentZone].ZoneCount = 2;
    AllZones[currentZone].AllVisibleZones[0] = 5;
    AllZones[currentZone].AllVisibleZones[1] = 6;
    AllZones[currentZone].id = currentZone;

    currentZone = 11;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 2;
    AllZones[currentZone].AllVisibleZones[1] = 3;
    AllZones[currentZone].AllVisibleZones[2] = 5;
    AllZones[currentZone].id = currentZone;

    currentZone = 12;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 1;
    AllZones[currentZone].AllVisibleZones[1] = 3;
    AllZones[currentZone].AllVisibleZones[2] = 4;
    AllZones[currentZone].id = currentZone;

    currentZone = 13;
    AllZones[currentZone].ZoneCount = 4;
    AllZones[currentZone].AllVisibleZones[0] = 1;
    AllZones[currentZone].AllVisibleZones[1] = 2;
    AllZones[currentZone].AllVisibleZones[2] = 3;
    AllZones[currentZone].AllVisibleZones[3] = 5;
    AllZones[currentZone].id = currentZone;

    currentZone = 14;
    AllZones[currentZone].ZoneCount = 5;
    AllZones[currentZone].AllVisibleZones[0] = 1;
    AllZones[currentZone].AllVisibleZones[1] = 2;
    AllZones[currentZone].AllVisibleZones[2] = 3;
    AllZones[currentZone].AllVisibleZones[3] = 4;
    AllZones[currentZone].AllVisibleZones[4] = 5;
    AllZones[currentZone].id = currentZone;

    currentZone = 15;
    AllZones[currentZone].ZoneCount = 4;
    AllZones[currentZone].AllVisibleZones[0] = 2;
    AllZones[currentZone].AllVisibleZones[1] = 3;
    AllZones[currentZone].AllVisibleZones[2] = 4;
    AllZones[currentZone].AllVisibleZones[3] = 5;
    AllZones[currentZone].id = currentZone;

    currentZone = 16;
    AllZones[currentZone].ZoneCount = 4;
    AllZones[currentZone].AllVisibleZones[0] = 1;
    AllZones[currentZone].AllVisibleZones[1] = 2;
    AllZones[currentZone].AllVisibleZones[2] = 3;
    AllZones[currentZone].AllVisibleZones[3] = 5;
    AllZones[currentZone].id = currentZone;

    currentZone = 17;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 1;
    AllZones[currentZone].AllVisibleZones[1] = 2;
    AllZones[currentZone].AllVisibleZones[2] = 3;
    AllZones[currentZone].id = currentZone;

    currentZone = 18;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 0;
    AllZones[currentZone].AllVisibleZones[1] = 1;
    AllZones[currentZone].AllVisibleZones[2] = 3;
    AllZones[currentZone].id = currentZone;

    currentZone = 19;
    AllZones[currentZone].ZoneCount = 3;
    AllZones[currentZone].AllVisibleZones[0] = 3;
    AllZones[currentZone].AllVisibleZones[1] = 4;
    AllZones[currentZone].AllVisibleZones[2] = 6;
    AllZones[currentZone].id = currentZone;

    currentZone = 20;
    AllZones[currentZone].ZoneCount = 4;
    AllZones[currentZone].AllVisibleZones[0] = 1;
    AllZones[currentZone].AllVisibleZones[1] = 3;
    AllZones[currentZone].AllVisibleZones[2] = 4;
    AllZones[currentZone].AllVisibleZones[3] = 6;
    AllZones[currentZone].id = currentZone;

    /////////////////////////////////////////////////////
    AllOcclusionZone[0].angles[0].x = -11.1;
    AllOcclusionZone[0].angles[0].y = 83.1;

    AllOcclusionZone[0].angles[1].x = -46.5;
    AllOcclusionZone[0].angles[1].y = 83.1;

    AllOcclusionZone[0].angles[2].x = -46.5;
    AllOcclusionZone[0].angles[2].y = 20.7;

    AllOcclusionZone[0].angles[3].x = -11.1;
    AllOcclusionZone[0].angles[3].y = 20.7;
    /////////////////////////////////////////////////////
    AllOcclusionZone[1].angles[0].x = -6.8;
    AllOcclusionZone[1].angles[0].y = 25.4;

    AllOcclusionZone[1].angles[1].x = -45.8;
    AllOcclusionZone[1].angles[1].y = 25.4;

    AllOcclusionZone[1].angles[2].x = -45.8;
    AllOcclusionZone[1].angles[2].y = -0.8;

    AllOcclusionZone[1].angles[3].x = -6.8;
    AllOcclusionZone[1].angles[3].y = -0.8;
    /////////////////////////////////////////////////////
    AllOcclusionZone[2].angles[0].x = -4.1;
    AllOcclusionZone[2].angles[0].y = 3.5;

    AllOcclusionZone[2].angles[1].x = -54.8;
    AllOcclusionZone[2].angles[1].y = 3.5;

    AllOcclusionZone[2].angles[2].x = -54.8;
    AllOcclusionZone[2].angles[2].y = -43;

    AllOcclusionZone[2].angles[3].x = -4.1;
    AllOcclusionZone[2].angles[3].y = -43;
    /////////////////////////////////////////////////////
    AllOcclusionZone[3].angles[0].x = 36;
    AllOcclusionZone[3].angles[0].y = 12.6;

    AllOcclusionZone[3].angles[1].x = -9.1;
    AllOcclusionZone[3].angles[1].y = 12.6;

    AllOcclusionZone[3].angles[2].x = -9.1;
    AllOcclusionZone[3].angles[2].y = -34.9;

    AllOcclusionZone[3].angles[3].x = 36;
    AllOcclusionZone[3].angles[3].y = -34.9;
    /////////////////////////////////////////////////////
    AllOcclusionZone[4].angles[0].x = 15.54;
    AllOcclusionZone[4].angles[0].y = 68.4;

    AllOcclusionZone[4].angles[1].x = -11.7;
    AllOcclusionZone[4].angles[1].y = 68.4;

    AllOcclusionZone[4].angles[2].x = -11.7;
    AllOcclusionZone[4].angles[2].y = 4.43;

    AllOcclusionZone[4].angles[3].x = 15.54;
    AllOcclusionZone[4].angles[3].y = 4.43;
    /////////////////////////////////////////////////////
    AllOcclusionZone[5].angles[0].x = 58.3;
    AllOcclusionZone[5].angles[0].y = 42;

    AllOcclusionZone[5].angles[1].x = 29.4;
    AllOcclusionZone[5].angles[1].y = 42;

    AllOcclusionZone[5].angles[2].x = 29.4;
    AllOcclusionZone[5].angles[2].y = -38.1;

    AllOcclusionZone[5].angles[3].x = 58.3;
    AllOcclusionZone[5].angles[3].y = -38.1;
    /////////////////////////////////////////////////////
    AllOcclusionZone[6].angles[0].x = 37.2;
    AllOcclusionZone[6].angles[0].y = 66.2;

    AllOcclusionZone[6].angles[1].x = 7.3;
    AllOcclusionZone[6].angles[1].y = 66.2;

    AllOcclusionZone[6].angles[2].x = 7.3;
    AllOcclusionZone[6].angles[2].y = 7.8;

    AllOcclusionZone[6].angles[3].x = 37.2;
    AllOcclusionZone[6].angles[3].y = 7.8;

    for (int i = 0; i < 7; i++)
    {
        for (int i2 = 0; i2 < 4; i2++)
        {
            AllOcclusionZone[i].anglesInt[i2].x = AllOcclusionZone[i].angles[i2].x * 8192.0;
            AllOcclusionZone[i].anglesInt[i2].y = AllOcclusionZone[i].angles[i2].y * 8192.0;
        }
    }
}

void AddAllCollisions()
{
    AllWallsCollisionsRef = GetWalls();
    CreateWall(9.38846, -1.4, -8.007592, 35.77919, 1, 33.08558, 3, 0);
    CreateWall(3.991, 0.285, 4.786, 2.423, 2.423, 2.423, 3, 1);
    CreateWall(5.41, 1.107, 5.980854, 0.4, 4, 30.68171, 3, 2);
    CreateWall(-1.4, 1.8, 38.985, 13, 1, 34.67, 4, 3);
    CreateWall(-10.31409, -1.03, 5.18, 17.45819, 1, 7, 1, 4);
    CreateWall(-1.654, 2.288648, 16.47623, 0.8, 6.477295, 19.37246, 4, 5);
    CreateWall(-1.654, 2.05, 2.617579, 0.8, 6, 1.884841, 3, 6);
    CreateWall(24.42, 1.8, 29.52468, 37.5, 1, 67.90935, -1, 7);
    CreateWall(48.3451, 1.8, 1.665, 9.130207, 1, 38.67, 5, 8);
    CreateWall(40.63582, 1.8, -14.36, 5.668356, 1, 7, 5, 9);
    CreateWall(44.5, 5.04, -33.965, 15, 1, 6.67, 5, 10);
    CreateWall(47.75, -3.04, 37.732, 9, 1, 7, 5, 11);
    CreateWall(40.68, 4.23, -23.46, 5.7, 1, 11.2, 5, 12);
    CreateWall(27.58, 4.225, -24.69, 20, 1, 9, 3, 13);
    CreateWall(21.51, 4.225, -12.405, 7, 1, 15.17, 3, 14);
    CreateWall(-30.05, 1.8, -16.65, 24.67, 1, 34, 2, 15);
    CreateWall(-37.12021, 2.6, -30.795, 9.679577, 1, 19.67, 2, 16);
    CreateWall(-38.66, 2.6065, 3.923, 10, 1, 16.17, 1, 17);
    CreateWall(-26.975, 2.6065, 10.75, 12.67, 1, 11.5, 1, 18);
    CreateWall(-31.85, 2.6065, 21.645, 16.8, 1, 9.97, 1, 19);
    CreateWall(-31.85, 1.8, 35.88, 16.8, 1, 16, 0, 20);
    CreateWall(-36.76, 5.03, 60.475, 6.5, 1, 6.67, 0, 21);
    CreateWall(-42.78, 5.03, 58.6, 5, 1, 10, 0, 22);
    CreateWall(-23.21, 5.03, 53.76, 20, 1, 19, 0, 23);
    CreateWall(-7.165, 5.03, 61.57, 11.17, 1, 10, 4, 24);
    CreateWall(-15.974, 0.591, 13.66, 3.7, 1, 3.37, 1, 25);
    CreateWall(55.00196, 3.415, 23.80224, 3.714117, 1, 17.83263, 5, 26);
    CreateWall(4.846629, 2.989377, -0.481, 0.9097431, 7.831755, 2.423, 3, 27);
    CreateWall(-0.8933716, 2.709331, -0.481, 0.9097431, 7.271662, 2.423, 3, 28);
    CreateWall(0.6405261, 1.114704, 0.3353194, 2.150795, 4.082407, 0.6293912, 3, 29);
    CreateWall(3.354151, 1.114704, -1.250537, 2.145238, 4.082407, 0.6865859, 3, 30);
    CreateWall(4.053317, 1.114704, -0.5631354, 0.6353655, 4.082407, 0.3297293, 3, 31);
    CreateWall(-0.1147662, 1.114704, -0.41167, 0.6778869, 4.082407, 0.3050682, 3, 32);
    CreateWall(-10.94747, 2.889332, -4.495358, 19.35818, 7.631665, 12.90928, -1, 33);
    CreateWall(-1.26794, 2.889332, -23.5255, 12.94932, 7.631665, 2.408997, 3, 34);
    CreateWall(-11.71794, 4.209399, -25.88165, 12.94932, 10.2718, 3.561691, 2, 35);
    CreateWall(-19.24169, 4.209399, -28.2181, 2.751635, 10.2718, 1.161465, 2, 36);
    CreateWall(-16.24241, 3.05695, -12.16243, 2.29025, 3.088099, 2.408759, 2, 37);
    CreateWall(-20.628, 4.339704, -12.831, 0.6293912, 4.082407, 2.150795, 2, 38);
    CreateWall(-21.486, 4.339704, -16.268, 0.3297293, 4.082407, 0.6353655, 2, 39);
    CreateWall(-22.215, 4.339704, -15.521, 0.6293912, 4.082407, 2.150795, 2, 40);
    CreateWall(-21.352, 4.339704, -12.076, 0.3297293, 4.082407, 0.6353655, 2, 41);
    CreateWall(-17.98957, 2.909, -11.5547, 1.207145, 1.2, 1.211931, 2, 42);
    CreateWall(-21.42, 5.894331, -11.297, 2.423, 7.271662, 0.9097431, 2, 43);
    CreateWall(-21.42, 5.894331, -17.06, 2.423, 7.271662, 0.9097431, 2, 44);
    CreateWall(-21.41365, 5.894331, -21.33369, 1.604465, 7.271662, 8.111914, 2, 45);
    CreateWall(-21.41365, 5.894331, -8.338291, 1.604465, 7.271662, 5.283774, 2, 46);
    CreateWall(-21.41365, 5.894331, -29.9949, 1.604465, 7.271662, 4.181724, 2, 47);
    CreateWall(-20.05958, 4.082152, -26.64717, 3.652607, 3.647305, 2.424164, 2, 48);
    CreateWall(-23.02708, 3.912082, -27.25182, 1.616625, 3.231766, 1.284817, 2, 49);
    CreateWall(-23.02901, 4.519009, -25.66659, 1.620493, 1.219922, 1.21636, 2, 50);
    CreateWall(-23.8313, 3.102336, -25.47028, 3.225069, 1.615268, 1.608994, 2, 51);
    CreateWall(9.870321, 1.493199, -8.82, 8.488289, 4.839397, 1, 3, 52);
    CreateWall(19.71741, 1.488101, -10.13818, 11.26746, 4.849592, 1.627839, 3, 53);
    CreateWall(34.23705, 3.871293, -0.4494247, 19.34229, 9.615976, 20.9745, -1, 54);
    CreateWall(8.829233, 0.6898834, -10.13725, 1.602546, 3.232767, 1.615795, 3, 55);
    CreateWall(7.217544, 0.2866741, -16.19459, 2.415168, 2.426349, 2.424468, 3, 56);
    CreateWall(6.816122, 2.310524, -16.59494, 1.612324, 1.596437, 1.616609, 3, 57);
    CreateWall(18.91403, 1.488101, -23.54818, 11.28471, 4.849592, 1, 3, 58);
    CreateWall(9.246572, 1.488101, -20.25092, 8.058916, 4.849592, 5.70549, 3, 59);
    CreateWall(24.96741, 1.488101, -20.2333, 0.8, 4.849592, 5.66225, 3, 60);
    CreateWall(26.58551, 0.4672852, -18.64375, 2.418206, 2.457632, 2.488899, 3, 61);
    CreateWall(29.00651, 1.064285, -18.64375, 2.418206, 2.457632, 2.488899, 3, 62);
    CreateWall(26.98624, 2.499154, -19.01415, 1.618732, 1.627894, 1.629699, 3, 63);
    CreateWall(31.60709, 2.795956, -20.02006, 12.55215, 5.453778, 0.4001274, 3, 64);
    CreateWall(40.70002, 3.903672, -17.60006, 6.458023, 3.238344, 0.4001274, 5, 65);
    CreateWall(37.69, 3.903672, -18.81139, 0.4001274, 3.238344, 2.822779, 5, 66);
    CreateWall(43.73, 3.903672, -23.85887, 0.4001274, 3.238344, 12.91774, 5, 67);
    CreateWall(25.17, 4.305497, -15.58066, 0.4001274, 2.434694, 9.287472, 3, 68);
    CreateWall(38.68263, 5.510857, -19.60757, 1.611956, 1.60449, 3.610546, 5, 69);
    CreateWall(38.68263, 7.150856, -19.01406, 1.611956, 1.60449, 1.619928, 5, 70);
    CreateWall(42.71363, 5.530857, -22.65006, 1.611956, 1.60449, 1.619928, 5, 71);
    CreateWall(27.79171, 7.936794, -32.73777, 19.3618, 6.416366, 8.091341, 3, 72);
    CreateWall(27.78882, 7.936794, -28.30295, 7.260923, 6.416366, 0.8257637, 3, 73);
    CreateWall(15.6907, 7.936794, -13.78543, 4.817293, 6.416366, 29.87009, 3, 74);
    CreateWall(18.42, 7.936794, -20.63499, 1, 6.416366, 7.24859, 3, 75);
    CreateWall(44.73563, 3.656087, -19.84385, 1.611956, 2.038029, 1.555517, 5, 76);
    CreateWall(54.45702, 6.858232, -27.74011, 4.934025, 8.573491, 20.66022, 5, 77);
    CreateWall(44.79, 6.858232, -37.25, 15, 8.573491, 1, 5, 78);
    CreateWall(43.12514, 3.871293, -10.13021, 3.204861, 9.615976, 3.212919, 5, 79);
    CreateWall(43.12514, 3.871293, 9.217745, 3.204861, 9.615976, 3.228837, 5, 80);
    CreateWall(55.61611, 4.50092, -5.702986, 2.438217, 2.458159, 2.414028, 5, 81);
    CreateWall(55.15685, 5.471698, 5.165548, 6.338552, 6.415167, 19.35178, 5, 82);
    CreateWall(58.07824, 5.471698, 7.948711, 2.521341, 6.415167, 50.76546, 5, 83);
    CreateWall(56.02677, 4.719615, 15.67324, 1.615543, 1.62077, 1.627525, 5, 84);
    CreateWall(55.63088, 6.345802, 31.40095, 2.447762, 4.873144, 2.418945, 5, 85);
    CreateWall(47.67071, 6.720139, 36.50803, 13.9532, 8.840049, 7.817391, 5, 86);
    CreateWall(37.47268, 6.720139, 35.01363, 3.218556, 8.840049, 1.644658, 5, 87);
    CreateWall(38.30424, 6.720139, 36.66363, 4.881684, 8.840049, 1.644658, 5, 88);
    CreateWall(47.88821, -0.1699834, 41.53085, 8.275005, 4.7418, 1.703053, 5, 89);
    CreateWall(43.52215, 0.2628939, 31.01274, 0.8107128, 5.686295, 19.40301, 5, 90);
    CreateWall(44.72398, -1.73808, 39.10096, 1.634037, 1.618159, 3.295925, 5, 91);
    CreateWall(51.18998, -1.73808, 35.84166, 1.634037, 1.618159, 3.231308, 5, 92);
    CreateWall(51.18998, -0.1290795, 36.23938, 1.634037, 1.618159, 1.610428, 5, 93);
    CreateWall(52.2789, 0.8512607, 29.83927, 0.5978088, 6.955806, 20.27887, 5, 94);
    CreateWall(52.2789, 3.373524, 15.67575, 0.5978088, 1.911279, 1.603443, 5, 95);
    CreateWall(33.44603, 6.720139, 36.03182, 4.826495, 8.840049, 29.46829, 6, 96);
    CreateWall(22.95895, 6.720139, 21.99475, 3.240649, 8.840049, 24.45416, 6, 97);
    CreateWall(30.64663, 5.934331, 33.42, 0.9097431, 7.271662, 2.423, 6, 98);
    CreateWall(24.90663, 5.934331, 33.42, 0.9097431, 7.271662, 2.423, 6, 99);
    CreateWall(26.44053, 4.339704, 32.63532, 2.150795, 4.082407, 0.6293912, 6, 100);
    CreateWall(29.15415, 4.339704, 34.19246, 2.145238, 4.082407, 0.6865859, 6, 101);
    CreateWall(29.85332, 4.339704, 33.49086, 0.6353655, 4.082407, 0.3297293, 6, 102);
    CreateWall(25.68523, 4.339704, 33.37033, 0.6778869, 4.082407, 0.3050682, 6, 103);
    CreateWall(29.813, 3.51, 24.547, 2.423, 2.423, 2.423, 6, 104);
    CreateWall(24.90663, 5.934331, 22.13, 0.9097431, 7.271662, 2.423, 6, 105);
    CreateWall(30.64663, 5.934331, 22.13, 0.9097431, 7.271662, 2.423, 6, 106);
    CreateWall(26.37633, 6.720139, 57.01281, 10.12095, 8.840049, 13.31212, 6, 107);
    CreateWall(24.96766, 5.934331, 50.29894, 0.8096883, 7.271662, 0.6728878, 6, 108);
    CreateWall(30.61666, 5.934331, 50.29894, 0.8096883, 7.271662, 0.6728878, 6, 109);
    CreateWall(21.266, 5.934331, 57.22, 0.6728878, 7.271662, 0.8096883, 6, 110);
    CreateWall(21.266, 5.934331, 62.85, 0.6728878, 7.271662, 0.8096883, 6, 111);
    CreateWall(15.38498, 6.720139, 65.11506, 13.95091, 8.840049, 3.702241, 6, 112);
    CreateWall(-18.70116, 6.720139, 67.90365, 54.33317, 8.840049, 2.839417, 0, 113);
    CreateWall(-20.62166, 9.071501, 66.18, 7.271675, 5.883001, 1, 0, 114);
    CreateWall(-33.51166, 9.071501, 66.18, 7.271675, 5.883001, 1, 0, 115);
    CreateWall(19.46931, 6.720139, 16.87856, 9.941291, 8.840049, 23.40178, -1, 116);
    CreateWall(11.42511, 6.720139, 16.45395, 6.092877, 8.840049, 22.55255, -1, 117);
    CreateWall(8.43699, 6.720139, 43.91084, 12.90911, 8.840049, 19.34007, -1, 118);
    CreateWall(15.2, 6.720139, 43.90626, 1, 8.840049, 7.272514, 6, 119);
    CreateWall(9.832866, 6.720139, 33.91, 10.07427, 8.840049, 1, 4, 120);
    CreateWall(3.994344, 3.108447, 33.42598, 1.615686, 1.604893, 1.623959, 4, 121);
    CreateWall(-5.272417, 5.538648, 25.75, 4.824834, 6.477295, 4, 4, 122);
    CreateWall(-5.272417, 5.538648, 36.227, 4.824834, 6.477295, 4, 4, 123);
    CreateWall(-6.90057, 6.730637, 46.3273, 11.26114, 8.861274, 20.97461, -1, 124);
    CreateWall(-7.831, 5.3, 30.99, 1, 6, 6.5, 4, 125);
    CreateWall(5.074815, 4.301765, 56.40557, 13.1601, 4.062151, 0.7663727, 4, 126);
    CreateWall(0.24, 7.568765, 47.13827, 3.5, 4.062151, 12.90346, 4, 127);
    CreateWall(-0.4394904, 4.314489, 43.90987, 1.612981, 4.03266, 3.21767, 4, 128);
    CreateWall(-11.73649, 7.54249, 64.4865, 1.612981, 4.03266, 4.060944, 0, 129);
    CreateWall(-11.92781, 7.54249, 56.60429, 1.995615, 4.03266, 2.005371, 0, 130);
    CreateWall(-14.16297, 7.145098, 55.19912, 3.225281, 3.237878, 3.219673, 0, 131);
    CreateWall(-13.34894, 7.145098, 52.7741, 1.610119, 3.237878, 1.614204, 0, 132);
    CreateWall(-11.92781, 7.54249, 62.85986, 1.995615, 4.03266, 0.8165054, 0, 133);
    CreateWall(-44.83156, 7.54249, 62.85986, 0.8431206, 4.03266, 0.8165054, 0, 134);
    CreateWall(-44.84383, 7.54249, 57.21986, 0.8676491, 4.03266, 0.8165054, 0, 135);
    CreateWall(-45.35478, 7.54249, 59.61, 1.089561, 4.03266, 15, 0, 136);
    CreateWall(-42.46, 5.924819, 39.06439, 5, 7.268002, 29.11122, 0, 137);
    CreateWall(-40.176, 5.523045, 55.21048, 0.4, 1.619911, 3.189045, 0, 138);
    CreateWall(-38.76395, 4.321394, 25.75254, 2.419907, 2.425211, 2.422911, 0, 139);
    CreateWall(-25.86095, 3.515394, 41.48711, 2.419907, 2.425211, 4.835773, 0, 140);
    CreateWall(-31.50165, 7.140826, 45.92187, 3.236599, 3.233137, 3.219515, 0, 141);
    CreateWall(-32.31604, 6.330677, 48.3438, 1.60783, 1.614568, 1.621651, 0, 142);
    CreateWall(-33.318, 4.250121, 50.35683, 0.4, 4.165757, 12.89635, 0, 143);
    CreateWall(-29.05922, 4.250121, 44.11132, 8.917554, 4.165757, 0.4053345, 0, 144);
    CreateWall(-24.24499, 6.744021, 40.68601, 0.8298302, 8.882464, 7.251068, 0, 145);
    CreateWall(-18.16362, 6.744021, 35.00214, 11.37258, 8.882464, 24.24732, 0, 146);
    CreateWall(-17.38598, 8.345907, 47.44, 7.268031, 5.678691, 1, 0, 147);
    CreateWall(-25.05, 7.125232, 19.71861, 7.251068, 8.120045, 6.440567, 1, 148);
    CreateWall(-35.55, 7.125232, 19.71861, 7.251068, 8.120045, 6.440567, 1, 149);
    CreateWall(-37.39254, 7.125231, 23.74257, 5.316154, 8.120045, 1.612665, 0, 150);
    CreateWall(-20.57564, 3.187231, 15.68, 12.98743, 8.120045, 1.612665, 1, 151);
    CreateWall(-38.3665, 5.120971, 14.08765, 9.724064, 4.111524, 4.898634, 1, 152);
    CreateWall(-43.49247, 5.120971, 8.41167, 0.5960007, 4.111524, 6.454079, 1, 153);
    CreateWall(-41.58795, 5.120971, 1.959855, 3.235893, 4.111524, 12.92029, 1, 154);
    CreateWall(-34.34674, 4.740863, 0.3402195, 4.858311, 4.87174, 9.681019, 1, 155);
    CreateWall(-30.24487, 4.740863, 2.731868, 3.448055, 4.87174, 4.779274, 1, 156);
    CreateWall(-19.33138, 5.148415, 5.109592, 21.90529, 4.056636, 6.620091, 1, 157);
    CreateWall(-8.102446, 3.298522, 11.64577, 12.11511, 7.756417, 6.451536, -1, 158);
    CreateWall(-9.72221, 0.6884941, 7.204192, 2.422421, 2.419012, 2.422384, 1, 159);
    CreateWall(-17.38029, 0.2831234, 2.711911, 3.224581, 1.60827, 1.728946, 1, 160);
    CreateWall(-19.47366, 1.283146, 5.166797, 0.9559011, 3.613672, 6.495212, 1, 161);
    CreateWall(-19.01047, 1.429199, 10.02323, 3.239193, 4.145567, 3.21852, 1, 162);
    CreateWall(-25.45899, 5.416459, -2.911342, 6.440163, 6.200087, 6.407383, 2, 163);
    CreateWall(-34.32658, 3.935917, -5.298461, 1.617161, 3.239004, 1.625077, 2, 164);
    CreateWall(-42.02054, 5.132627, -21.06406, 0.8790817, 5.646835, 33.14563, 2, 165);
    CreateWall(-41.19003, 5.970626, -28.69818, 0.7959557, 5.646835, 7.276354, 2, 166);
    CreateWall(-41.38817, 5.129626, -11.3631, 0.4163437, 5.646835, 0.7861614, 2, 167);
    CreateWall(-41.38817, 5.129626, -16.9751, 0.4163437, 5.646835, 0.7861614, 2, 168);
    CreateWall(-39.97736, 3.099399, -20.8255, 3.224247, 1.623342, 0.3831959, 2, 169);
    CreateWall(-33.71442, 3.099399, -20.83454, 2.830128, 1.623342, 0.4012718, 2, 170);
    CreateWall(-33.92458, 3.504645, -19.42433, 2.409798, 2.433834, 2.412289, 2, 171);
    CreateWall(-32.10999, 3.099732, -25.9817, 0.4169121, 1.624008, 11.07064, 2, 172);
    CreateWall(-29.48116, 3.909713, -23.45477, 1.612955, 3.243968, 1.615158, 2, 173);
    CreateWall(-40.17027, 4.314253, -31.31842, 1.213169, 2.440887, 1.210466, 2, 174);
    CreateWall(-40.78803, 4.711897, -38.37749, 1.635225, 3.236175, 1.613446, 2, 175);
    CreateWall(-37.95386, 4.711897, -39.17149, 4.044044, 3.236175, 1.613446, 2, 176);
    CreateWall(-37.14289, 5.310734, -37.17551, 2.42997, 4.433849, 2.441495, 2, 177);
    CreateWall(-29.07345, 4.917582, -33.95314, 13.76087, 5.220153, 4.064755, 2, 178);
    CreateWall(-26.675, 5.129626, -31.92304, 0.7861614, 5.646835, 0.8204193, 2, 179);
    CreateWall(-32.3, 5.129626, -31.92304, 0.7861614, 5.646835, 0.8204193, 2, 180);
    CreateWall(-40.78566, 3.90744, -21.84602, 1.629536, 1.62726, 1.619009, 2, 181);
    CreateWall(15.65053, 3.52211, -1.713665, 4.895332, 2.395253, 5.782387, 3, 182);
    CreateWall(12.46407, 3.925133, 1.150944, 1.617191, 3.201298, 1.618112, 3, 183);
    CreateWall(5.992119, 5.469345, -0.4590559, 14.66269, 6.289724, 1.618112, 3, 184);
    CreateWall(23.35082, 3.525312, 3.975177, 2.418831, 2.389922, 2.420465, 3, 185);
    CreateWall(23.75113, 6.332403, -9.739486, 1.617455, 3.222512, 1.611141, 3, 186);
    CreateWall(21.54216, 3.514502, -2.880072, 0.4025955, 2.410711, 3.235695, 3, 187);
    CreateWall(-19.39825, 3.717201, -23.05369, 2.410248, 2.821984, 2.427719, 2, 188);
    CreateWall(-17.58567, 3.108061, -23.05369, 1.205088, 1.603703, 2.427719, 2, 189);
    CreateWall(-11.74238, 1.165568, -22.24077, 1.60723, 2.26469, 1.611542, 2, 190);
    CreateWall(-10.12338, 0.5635681, -23.04477, 1.60723, 2.26469, 1.611542, 2, 191);
    CreateWall(-10.12338, 0.371568, -21.43977, 1.60723, 2.26469, 1.611542, 2, 192);
    CreateWall(-24.23902, 2.723958, -26.694, 0.8139538, 0.8524289, 0.8340797, 2, 193);
    CreateWall(-17.78772, 2.671109, -21.45288, 0.8073368, 0.7361269, 0.775835, 2, 194);
    CreateWall(32.64186, 3.912763, 11.63979, 3.213718, 3.226475, 3.216652, 6, 195);
    CreateWall(35.05, 3.102, 10.828, 1.62, 1.62, 1.62, 5, 196);
    CreateWall(23.153, 3.307, -4.994, 2.82, 2, 1, 3, 197);
    CreateWall(-20.6154, 4.318905, 9.235, 3.226804, 1.613811, 1.6, 1, 198);
    CreateWall(8.033, 2.504, 0.538, 4.05, 0.4, 1.222159, 3, 199);
    CreateWall(18.112, 2.504, 28.781, 4.05, 0.4, 1.222159, 6, 200);
    CreateWall(15.9, 2.504, 43.898, 1.222159, 0.4, 4.05, 6, 201);
    CreateWall(-21.4125, 8.321119, -26.54848, 1.607979, 0.8150377, 4.404967, 2, 202);
    CreateWall(-25.052, 2.665476, -24.29729, 0.8, 0.7450484, 0.7311573, 2, 203);
    CreateWall(51.25, -0.15, 40.724, 1.77, 4.7418, 1.703053, 5, 204);
    CreateWall(43.89105, 0.4453565, 40.36826, 1.712103, 3.551087, 0.9915719, 5, 205);
    CreateWall(56.639, 4.118, 26.56685, 1.222159, 0.4, 4.037703, 5, 206);
    CreateWall(-20.6154, 6.148234, 9.235, 3.226804, 2.025973, 1.6, 1, 207);
    CreateWall(-21.006, 6.454825, 14.67238, 0.8, 1.41279, 0.415242, 1, 208);
    CreateWall(-21.006, 6.713649, 14.14657, 0.8, 0.895143, 0.8783064, 1, 209);
}

void AddAllStairs()
{
    AllStairsRef = GetStairs();
    CreateStairs(-1.257, 5.2, 8.419, 21.325, 0, 3.211, 2, 0);
    CreateStairs(-1.252, -0.85, 3.576, 6.8, 0, 0.388, 1, 1);
    CreateStairs(5.3, 6.45, 21.313, 56.32, 3.24, 3.24, 1, 2);
    CreateStairs(41.98, 45.06, 10.7, 21.311, 3.24, 3.24, 2, 3);
    CreateStairs(24.558, 37.4724, -20.03, -10.9489, 0, 3.2112, 3, 4);
    CreateStairs(42.87, 44.44, -17.4, -11.755, 3.24, 3.24, 2, 5);
    CreateStairs(50.37, 56.8304, -17.4011, -4.494, 3.237, 4.8242, 3, 6);
    CreateStairs(43.93, 51.99, -30.31, -17.403, 3.228, 6.451, 0, 7);
    CreateStairs(43.93, 51.99, 21.315, 34.231, -1.613, 3.217, 0, 8);
    CreateStairs(49.58, 52.804, 16.485, 19.704, 3.228, 4.829, 3, 9);
    CreateStairs(18.111, 21.33, -4.495, 0.33, 3.237, 5.64, 0, 10);
    CreateStairs(37.48, 43.51, -30.31, -28.293, 5.647, 6.46, 0, 11);
    CreateStairs(37.13, 38.37, -28.69, -20.22, 5.632, 5.632, 2, 12);
    CreateStairs(18.89, 24.98, -20.98, -19.1, 5.632, 5.632, 3, 13);
    CreateStairs(-17.385, -7.699999, -23.885, -10.9489, 0, 3.211, 1, 14);
    CreateStairs(-38.35, -35.131, -20.628, -19.037, 3.237, 4.026, 0, 15);
    CreateStairs(-39.974, -36.749, -6.12, -4.495998, 3.237, 4.026, 2, 16);
    CreateStairs(-34.28, -33.04, 4.909999, 13.38, 4.02, 4.02, 2, 17);
    CreateStairs(-32.48, -28.09, 15.51, 17.13, 4.02, 4.02, 3, 18);
    CreateStairs(-39.972, -23.841, 26.966, 28.46, 3.228, 4.019, 0, 19);
    CreateStairs(-39.97, -33.52, 43.904, 56.818, 3.23, 6.449, 2, 20);
    CreateStairs(-1.248, 11.659, 56.79, 66.49001, 3.227, 6.441, 1, 21);
    CreateStairs(-33.96, -32.95, 56.824, 63.264, 6.44, 6.44, 0, 22);
    CreateStairs(-40.67, -39.49, 56.824, 63.264, 6.44, 6.44, 0, 23);
    CreateStairs(-13.74, -12.56, 56.824, 63.264, 6.44, 6.44, 0, 24);
    CreateStairs(-44.813, -12.546, 63.2688, 66.496, 6.454, 7.255, 2, 25);
    CreateStairs(-17.383, -14.166, 8.419, 11.642, 0.4100001, 2.006, 2, 26);
    CreateStairs(-20.3, -17.214, 11.644, 14.868, 2.073, 4.018, 1, 27);
    CreateStairs(-20.585, -18.3, -25.084, -23.196, 6.058, 6.893, 0, 28);
    CreateStairs(-22.7, -22.23, -27.867, -26.293, 6.457, 6.843, 3, 29);
    CreateStairs(-23.845, -22.226, -26.263, -25.927, 6.052, 6.439, 0, 30);
}

void CreateStairs(float xSideA, float xSideB, float zSideA, float zSideB, float startY, float endY, int direction, int index)
{
    Stairs *newStairs = &AllStairsRef[index];

    newStairs->xSideA = xSideA;
    newStairs->xSideB = xSideB;
    newStairs->zSideA = zSideA;
    newStairs->zSideB = zSideB;
    newStairs->startY = startY;
    newStairs->endY = endY;
    newStairs->direction = direction;
}

void CreateWall(float xPos, float yPos, float zPos, float xSize, float ySize, float zSize, int Zone, int index)
{
    Wall *newWall = &AllWallsCollisionsRef[index];

    newWall->WallModel = NE_ModelCreate(NE_Static);
    newWall->WallPhysics = NE_PhysicsCreate(NE_BoundingBox);
    newWall->WallPhysics->physicsgroupCount = 2;
    newWall->WallPhysics->physicsgroup[1] = 1;
    NE_ModelSetCoord(newWall->WallModel, xPos, yPos, zPos);
    NE_PhysicsSetModel(newWall->WallPhysics, (void *)newWall->WallModel); // Physics object and Model assigned to it
    NE_PhysicsSetSize(newWall->WallPhysics, xSize, ySize, zSize);
    newWall->xPos = xPos * 8192.0;
    newWall->yPos = yPos * 8192.0;
    newWall->zPos = zPos * 8192.0;
    newWall->ZoneCollision = Zone;
    newWall->WallCollisionBox.xSize = newWall->WallPhysics->xsize;
    newWall->WallCollisionBox.ySize = newWall->WallPhysics->ysize;
    newWall->WallCollisionBox.zSize = newWall->WallPhysics->zsize;
    CalculateWallColBox(index);
    NE_PhysicsEnable(newWall->WallPhysics, false);
}

// Calculate a box from physics value for raycasting
void CalculatePlayerColBox(int PlayerId)
{
    CalculatePlayerPosition(PlayerId);
    Player *player = &AllPlayers[PlayerId];

    float xSize = player->PlayerCollisionBox.xSize;
    float ySize = player->PlayerCollisionBox.ySize;
    float zSize = player->PlayerCollisionBox.zSize;
    player->PlayerCollisionBox.BoxXRangeA = (player->xPos + xSize) * 4096.0;
    player->PlayerCollisionBox.BoxXRangeB = (player->xPos - xSize) * 4096.0;
    player->PlayerCollisionBox.BoxYRangeA = (player->yPos + ySize) * 4096.0;
    player->PlayerCollisionBox.BoxYRangeB = (player->yPos - ySize) * 4096.0;
    player->PlayerCollisionBox.BoxZRangeA = (player->zPos + zSize) * 4096.0;
    player->PlayerCollisionBox.BoxZRangeB = (player->zPos - zSize) * 4096.0;
}

void CalculateWallColBox(int WallId)
{
    Wall *newWall = &AllWallsCollisionsRef[WallId];

    float xSize = newWall->WallCollisionBox.xSize;
    float ySize = newWall->WallCollisionBox.ySize;
    float zSize = newWall->WallCollisionBox.zSize;
    newWall->WallCollisionBox.BoxXRangeA = newWall->xPos + xSize;
    newWall->WallCollisionBox.BoxXRangeB = newWall->xPos - xSize;
    newWall->WallCollisionBox.BoxYRangeA = newWall->yPos + ySize;
    newWall->WallCollisionBox.BoxYRangeB = newWall->yPos - ySize;
    newWall->WallCollisionBox.BoxZRangeA = newWall->zPos + zSize;
    newWall->WallCollisionBox.BoxZRangeB = newWall->zPos - zSize;
}

void CalculateTriggerColBox(float xPos, float zPos, float xSize, float zSize, int TriggerId)
{
    CollisionBox2D *trigger = &AllTriggersCollisions[TriggerId];
    trigger->BoxXRangeA = xPos + xSize / 2.0;
    trigger->BoxXRangeB = xPos - xSize / 2.0;
    trigger->BoxZRangeA = zPos + zSize / 2.0;
    trigger->BoxZRangeB = zPos - zSize / 2.0;
}

void SetBombZone(float xPos, float zPos, float xSize, float zSize, int TriggerId, CollisionBox2D AllBombsTriggersCollisions[])
{
    CollisionBox2D *trigger = &AllBombsTriggersCollisions[TriggerId];
    trigger->BoxXRangeA = xPos + xSize / 2.0;
    trigger->BoxXRangeB = xPos - xSize / 2.0;
    trigger->BoxZRangeA = zPos + zSize / 2.0;
    trigger->BoxZRangeB = zPos - zSize / 2.0;
}

void SetBombDefuseZone(float xPos, float zPos, float xSize, float zSize, CollisionBox2D *DefuseZoneCollisions)
{
    DefuseZoneCollisions->BoxXRangeA = xPos + xSize / 2.0;
    DefuseZoneCollisions->BoxXRangeB = xPos - xSize / 2.0;
    DefuseZoneCollisions->BoxZRangeA = zPos + zSize / 2.0;
    DefuseZoneCollisions->BoxZRangeB = zPos - zSize / 2.0;
}

void checkPlayerOcclusionZone(int playerIndex, int playerCameraTarget)
{
    // Player *player = &AllPlayers[playerIndex];
    // Player *player = &AllPlayers[playerIndex];
    if (AllPlayersRefForCollisions[playerCameraTarget].xPos <= AllTriggersCollisions[AllPlayers[playerIndex].CurrentOcclusionZone].BoxXRangeA && AllPlayersRefForCollisions[playerCameraTarget].xPos >= AllTriggersCollisions[AllPlayers[playerIndex].CurrentOcclusionZone].BoxXRangeB && AllPlayersRefForCollisions[playerCameraTarget].zPos <= AllTriggersCollisions[AllPlayers[playerIndex].CurrentOcclusionZone].BoxZRangeA && AllPlayersRefForCollisions[playerCameraTarget].zPos >= AllTriggersCollisions[AllPlayers[playerIndex].CurrentOcclusionZone].BoxZRangeB)
    {
        // printf("HEY");
        //*CurrentOcclusionZone = TriggersIndex;
        // break;
    }
    else
    {
        // Check occlusion zones
        for (int TriggersIndex = 0; TriggersIndex < OcclusionZonesCount; TriggersIndex++)
        {
            if (AllPlayersRefForCollisions[playerCameraTarget].xPos <= AllTriggersCollisions[TriggersIndex].BoxXRangeA && AllPlayersRefForCollisions[playerCameraTarget].xPos >= AllTriggersCollisions[TriggersIndex].BoxXRangeB && AllPlayersRefForCollisions[playerCameraTarget].zPos <= AllTriggersCollisions[TriggersIndex].BoxZRangeA && AllPlayersRefForCollisions[playerCameraTarget].zPos >= AllTriggersCollisions[TriggersIndex].BoxZRangeB)
            {
                AllPlayers[playerIndex].CurrentOcclusionZone = TriggersIndex;
                break;
            }
        }
    }
}

void CheckZones(CollisionBox2D AllTriggersCollisions[], CollisionBox2D AllBombsTriggersCollisions[], CollisionBox2D bombDefuseZone, bool *CanPutBomb, bool *canDefuseBomb)
{
    checkPlayerOcclusionZone(0, GetCurrentCameraPlayer());

    for (int i = 1; i < MaxPlayer; i++)
    {
        if (AllPlayers[i].Id != -1)
            checkPlayerOcclusionZone(i, i);
    }

    // Check bomb zones
    for (int TriggersIndex = 0; TriggersIndex < 2; TriggersIndex++)
        if (localPlayer->xPos <= AllBombsTriggersCollisions[TriggersIndex].BoxXRangeA && localPlayer->xPos >= AllBombsTriggersCollisions[TriggersIndex].BoxXRangeB && localPlayer->zPos <= AllBombsTriggersCollisions[TriggersIndex].BoxZRangeA && localPlayer->zPos >= AllBombsTriggersCollisions[TriggersIndex].BoxZRangeB)
        {
            *CanPutBomb = true;
            break;
        }

    if (localPlayer->xPos <= bombDefuseZone.BoxXRangeA && localPlayer->xPos >= bombDefuseZone.BoxXRangeB && localPlayer->zPos <= bombDefuseZone.BoxZRangeA && localPlayer->zPos >= bombDefuseZone.BoxZRangeB)
        *canDefuseBomb = true;
}

int LastStairs = 0;

void CheckStairs(int StairsCount, int *CanJump, bool *isInDownStairs)
{
    bool firstScan = true;
    for (int i = LastStairs; i < StairsCount; i++)
    {
        Stairs *stairs = &AllStairsRef[i];
        // bool IsOn = AllPlayersRefForCollisions[0].zPos >= AllStairsRef[i].zSideA && AllPlayersRefForCollisions[0].zPos <= AllStairsRef[i].zSideB && AllPlayersRefForCollisions[0].xPos >= AllStairsRef[i].xSideA && AllPlayersRefForCollisions[0].xPos <= AllStairsRef[i].xSideB;
        if (localPlayer->zPos >= stairs->zSideA && localPlayer->zPos <= stairs->zSideB && localPlayer->xPos >= stairs->xSideA && localPlayer->xPos <= stairs->xSideB)
        {
            float yVal = 0;
            if (stairs->direction == 0)
                yVal = map(localPlayer->zPos, stairs->zSideA, stairs->zSideB, stairs->endY, stairs->startY);
            else if (stairs->direction == 1)
                yVal = map(localPlayer->xPos, stairs->xSideA, stairs->xSideB, stairs->endY, stairs->startY);
            else if (stairs->direction == 2)
                yVal = map(localPlayer->zPos, stairs->zSideA, stairs->zSideB, stairs->startY, stairs->endY);
            else
                yVal = map(localPlayer->xPos, stairs->xSideA, stairs->xSideB, stairs->startY, stairs->endY);

            if (localPlayer->yPos < yVal && yVal - localPlayer->yPos < 3)
            {
                if (localPlayer->PlayerPhysic->yspeed < 100)
                    localPlayer->PlayerPhysic->yspeed = 0;

                localPlayer->yPos = yVal;
                NE_ModelSetCoord(localPlayer->PlayerModel, localPlayer->xPos, localPlayer->yPos, localPlayer->zPos);
                *CanJump = 10;
            }
            if (localPlayer->yPos == yVal)
                *isInDownStairs = true;

            LastStairs = i;
            break;
        }
        if (firstScan)
        {
            firstScan = false;
            LastStairs = 0;
            i = 0;
        }
    }
}

bool checkZoneForOcclusion(OcclusionZone *zone, int posx, int posz)
{
    if (zone->anglesInt[0].x >= posx && zone->anglesInt[2].x <= posx)
    {
        if (zone->anglesInt[0].y >= posz && zone->anglesInt[2].y <= posz)
        {
            return true;
        }
    }
    return false;
}

// int LastStairs = 0;

bool CheckStairsForGrenades(PhysicalGrenade *grenade, int StairsCount)
{
    bool firstScan = true;
    for (int i = grenade->LastStairs; i < StairsCount; i++)
    // for (int i = 0; i < StairsCount; i++)
    {
        float xpos = grenade->Model->x / 4096.0;
        float ypos = grenade->Model->y / 4096.0;
        float zpos = grenade->Model->z / 4096.0;
        Stairs *stairs = &AllStairsRef[i];

        if (zpos >= stairs->zSideA && zpos <= stairs->zSideB && xpos >= stairs->xSideA && xpos <= stairs->xSideB)
        {
            float yVal = 0;
            if (stairs->direction == 0)
            {
                yVal = map(zpos, stairs->zSideA, stairs->zSideB, stairs->endY, stairs->startY);
            }
            else if (stairs->direction == 1)
            {
                yVal = map(xpos, stairs->xSideA, stairs->xSideB, stairs->endY, stairs->startY);
            }
            else if (stairs->direction == 2)
            {
                yVal = map(zpos, stairs->zSideA, stairs->zSideB, stairs->startY, stairs->endY);
            }
            else
            {
                yVal = map(xpos, stairs->xSideA, stairs->xSideB, stairs->startY, stairs->endY);
            }

            if (ypos < yVal && yVal - ypos < 3)
            {
                if (grenade->Physic->yspeed < 100)
                    grenade->Physic->yspeed = 0;

                NE_ModelSetCoord(grenade->Model, xpos, yVal - 0.6, zpos);

                if (stairs->startY != stairs->endY)
                {
                    if (stairs->direction == 0)
                    {
                        grenade->Physic->zspeed += 15;
                    }
                    else if (stairs->direction == 1)
                    {
                        grenade->Physic->xspeed += 15;
                    }
                    else if (stairs->direction == 2)
                    {
                        grenade->Physic->zspeed -= 15;
                    }
                    else
                    {
                        grenade->Physic->xspeed -= 15;
                    }
                }
            }
            grenade->LastStairs = i;
            return true;
        }
        if (firstScan)
        {
            firstScan = false;
            grenade->LastStairs = 0;
            i = 0;
        }
    }
    return false;
}

void prepareAiRaycast(int fromPlayerIndex, int toPlayerIndex, bool checkVisibility)
{
    Player *shooterPlayer = &AllPlayers[fromPlayerIndex];
    Player *targetPlayer = &AllPlayers[toPlayerIndex];

    float distance2D = sqrtf(powf(targetPlayer->PlayerModel->x - shooterPlayer->PlayerModel->x, 2.0) + powf(targetPlayer->PlayerModel->z - shooterPlayer->PlayerModel->z, 2.0));
    Vector3 Direction;
    Direction.x = targetPlayer->PlayerModel->x - shooterPlayer->PlayerModel->x;
    Direction.y = targetPlayer->PlayerModel->y - (shooterPlayer->PlayerModel->y + CameraOffsetY * 4096.0);
    Direction.z = targetPlayer->PlayerModel->z - shooterPlayer->PlayerModel->z;
    shooterPlayer->AngleDestination = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 256.0;
    shooterPlayer->Angle = shooterPlayer->AngleDestination;
    float CameraAngleToGrenadeDirection = atan2f(distance2D, Direction.y) * 512.0 / (M_TWOPI);

    float x2 = 0, y2 = 0, z2 = 0;
    UpdateLookRotationAI(CameraAngleToGrenadeDirection, fromPlayerIndex, &x2, &y2, &z2);

    if (!checkVisibility)
    {
        if ((targetPlayer->isAi && targetPlayer->target != -1) || (!targetPlayer->isAi && targetPlayer->PlayerPhysic->xspeed + targetPlayer->PlayerPhysic->yspeed + targetPlayer->PlayerPhysic->zspeed == 0))
        {
            x2 += ((rand() % 200) - 100) / 1600.0;
            y2 += ((rand() % 200) - 100) / 1600.0;
            z2 += ((rand() % 200) - 100) / 1600.0;
        }
        else
        {
            x2 += ((rand() % 200) - 100) / 900.0;
            y2 += ((rand() % 200) - 100) / 900.0;
            z2 += ((rand() % 200) - 100) / 900.0;
        }
    }

    CalculatePlayerPosition(fromPlayerIndex);
    shooterPlayer->startXPosRaycast = shooterPlayer->xPos;
    shooterPlayer->startYPosRaycast = shooterPlayer->yPos;
    shooterPlayer->startZPosRaycast = shooterPlayer->zPos;

    shooterPlayer->startXRotRaycast = x2;
    shooterPlayer->startYRotRaycast = y2;
    shooterPlayer->startZRotRaycast = z2;
    shooterPlayer->startGunIdRaycast = shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory];

    shooterPlayer->justCheking = checkVisibility;
}

int Raycast(int playerId)
{
    Player *shooterPlayer = &AllPlayers[playerId];
    shooterPlayer->IsHeadShot = false;
    shooterPlayer->IsLegShot = false;
    int HitPlayerId = -1;

    // Calculate for each player the collision box
    for (int PlayerIndex = 0; PlayerIndex < MaxPlayer; PlayerIndex++) // TODO if index given != current index
        if (PlayerIndex != playerId)
            CalculatePlayerColBox(PlayerIndex);

    // int ForCount = 3 * RaycastDistance;
    int ForCount = 150;

    // if (shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory] < GunCount && AllGuns[shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory]].isKnife)
    if (AllGuns[shooterPlayer->startGunIdRaycast].isKnife) // ICI
        ForCount = 4;                                      // ICI
    // else if (shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory] >= GunCount + shopGrenadeCount /* && AllGuns[AllGunsInInventory[currentGunInInventory]].isKnife*/)
    // ForCount = 5;

    shooterPlayer->PlayerFoundAtDistance = -1;

    // Check players raycast if there are more than one player
    if (PlayerCount > 1)
    {
        for (int i = 0; i < ForCount; i++)
        {
            float Step = i / 2.5; // Set RayAccuracy to 3.0 (lower RayAccuracy, lower accuracy but higher scan distance, higher RayAccuracy, higher accuracy but lower scan distance )
            // Calculate next raycast point
            int NextXRayPoint = (shooterPlayer->startXPosRaycast + shooterPlayer->startXRotRaycast * Step) * 4096, NextYRayPoint = (shooterPlayer->startYPosRaycast + CameraOffsetY + shooterPlayer->startYRotRaycast * Step) * 4096, NextZRayPoint = (shooterPlayer->startZPosRaycast + shooterPlayer->startZRotRaycast * Step) * 4096;

            // Check if next raycast point is in a player body
            for (int PlayerIndex = 0; PlayerIndex < MaxPlayer; PlayerIndex++)
            { // for (int PlayerIndex = 0; PlayerIndex < 2; PlayerIndex++)
                Player *testedPlayer = &AllPlayers[PlayerIndex];
                if (testedPlayer->Id != -1 && PlayerIndex != playerId && !testedPlayer->IsDead && NextXRayPoint <= testedPlayer->PlayerCollisionBox.BoxXRangeA && NextXRayPoint >= testedPlayer->PlayerCollisionBox.BoxXRangeB && NextYRayPoint <= testedPlayer->PlayerCollisionBox.BoxYRangeA && NextYRayPoint >= testedPlayer->PlayerCollisionBox.BoxYRangeB && NextZRayPoint <= testedPlayer->PlayerCollisionBox.BoxZRangeA && NextZRayPoint >= testedPlayer->PlayerCollisionBox.BoxZRangeB)
                {
                    if (shooterPlayer->isAi && testedPlayer->isAi) // TODO MOVE THIS SUPER OPTIMISED CODE
                    {
                        CalculatePlayerPosition(playerId);
                        CalculatePlayerPosition(PlayerIndex);
                        if (GetDistanceBewteenTwoPlayers(PlayerIndex, playerId) < 2)
                        {
                            continue;
                        }
                    }

                    if (testedPlayer->IsCounter == shooterPlayer->IsCounter && !allPartyModes[currentPartyMode].teamDamage)
                        break;

                    if (NextYRayPoint - testedPlayer->PlayerCollisionBox.BoxYRangeB >= 5480)
                    {
                        shooterPlayer->IsHeadShot = true;
                        // printf("Headshot ");
                    }
                    else if (NextYRayPoint - testedPlayer->PlayerCollisionBox.BoxYRangeB <= 2800)
                    {
                        shooterPlayer->IsLegShot = true;
                        // printf("LegShot ");
                    }

                    // printf("Hit y %d\n", NextYRayPoint - testedPlayer->PlayerCollisionBox.BoxYRangeB);
                    // printf("%d Hits player %d at %d\n", playerId, PlayerIndex, i);
                    HitPlayerId = PlayerIndex;
                    shooterPlayer->PlayerFoundAtDistance = i;
                    /*if (applyRules)
                    {
                        Hit = HitPlayerId;
                    }*/
                    i = ForCount;
                    break;
                }
            }
        }
    }

    // if (HitPlayerId == -1)
    // shooterPlayer->PlayerFoundAtDistance = -1;

    // if (/*shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory] < GunCount && */ AllGuns[shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory]].isKnife)

    // else if (shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory] >= GunCount + shopGrenadeCount) //TODO ??????? Maybe for the Taser
    // PlayerFoundAtDistance = 5;

    // Prepare next scan for walls
    shooterPlayer->OldStopAt = 0;
    shooterPlayer->StopAt = 30;
    shooterPlayer->ScanFinished = false;

    if (AllGuns[shooterPlayer->startGunIdRaycast].isKnife) // ICI
        shooterPlayer->PlayerFoundAtDistance = 4;          // ICI

    if (CheckWallHit(playerId) == -1) // If a wall is detected before the player
    {
        HitPlayerId = -1;
        shooterPlayer->ScanFinished = true;
    }
    // else if (shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory] < GunCount && !AllGuns[shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory]].isKnife)
    else if (!AllGuns[shooterPlayer->startGunIdRaycast].isKnife)
    {
        shooterPlayer->OldStopAt = shooterPlayer->StopAt;
        shooterPlayer->StopAt += 30;
    }
    // else if (shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory] < GunCount && AllGuns[shooterPlayer->AllGunsInInventory[shooterPlayer->currentGunInInventory]].isKnife) //Play normal knife sound if no wall/player was detected
    else if (!shooterPlayer->justCheking)
    {
        PlayBasicSound(AllGuns[shooterPlayer->startGunIdRaycast].gunSound);
    }

    // Return no object/player hit
    return HitPlayerId;
}

int CheckWallHit(int playerId)
{
    int NextXRayPoint = 0, NextYRayPoint = -10, NextZRayPoint = 0;
    int LastNextXRayPoint, LastNextYRayPoint, LastNextZRayPoint;

    // Make a list of all wall to check (all wall in a visible zone for the player)
    int WallCountToTest = 0;
    int AllWallsToCheck[wallCount];
    Player *player = &AllPlayers[playerId];
    for (int i2 = 0; i2 < wallCount; i2++)
    {
        if (AllWallsCollisions[i2].ZoneCollision != -1)
        {
            for (int i3 = 0; i3 < AllZones[player->CurrentOcclusionZone].ZoneCount; i3++)
            {
                if (AllZones[player->CurrentOcclusionZone].AllVisibleZones[i3] == AllWallsCollisions[i2].ZoneCollision)
                {
                    AllWallsToCheck[WallCountToTest] = i2;
                    WallCountToTest++;
                    break;
                }
            }
        }
        else
        {
            AllWallsToCheck[WallCountToTest] = i2;
            WallCountToTest++;
        }
    }

    // printf("WallCount %d", StopAt);

    for (int i = player->OldStopAt; i < player->StopAt; i++)
    {
        if (player->PlayerFoundAtDistance == i)
        {
            // printf("STOP %d hit %d\n", i, Hit);
            player->ScanFinished = true;

            if (player->isAi)
            {
                if (player->justCheking)
                {
                    player->lastSeenTarget = player->target;
                }
                else
                {
                    // int newHealth = AllPlayers[player->target].Health - AllGuns[player->startGunIdRaycast].Damage;
                    // setPlayerHealth(player->target, newHealth);
                    // AllPlayers[player->target].Health--;
                    // checkAfterDamage(playerId, player->target, true);
                    makeHit(playerId, player->target);

                    /*if (AllPlayers[player->target].Health == 0)
                    {
                        player->KillCount++;
                        AllPlayers[player->target].DeathCount++;
                        if (currentMenu == 2)
                            UpdateBottomScreenOneFrame += 8;
                    }*/
                }
            }
            /*else if (applyRules)
            {
                makeHit(playerId, Hit);
                //int newHealth = AllPlayers[Hit].Health - AllGuns[player->startGunIdRaycast].Damage;
                //setPlayerHealth(Hit, newHealth);
                Hit = -1;
                //AllPlayers[Hit].Health--;
            }*/

            return 0;
        }

        // set raycast point step
        float Step = i / 2.5;
        // Calculate next raycast point
        LastNextXRayPoint = NextXRayPoint;
        LastNextYRayPoint = NextYRayPoint;
        LastNextZRayPoint = NextZRayPoint;

        NextXRayPoint = (player->startXPosRaycast + player->startXRotRaycast * Step) * 8192;
        NextYRayPoint = (player->startYPosRaycast + CameraOffsetY + player->startYRotRaycast * Step) * 8192;
        NextZRayPoint = (player->startZPosRaycast + player->startZRotRaycast * Step) * 8192;

        // Check static objects
        for (int i2 = 0; i2 < WallCountToTest; i2++)
        {
            // Check if next raycast point is in a wall
            int WallIndex = AllWallsToCheck[i2];
            if (NextXRayPoint <= AllWallsCollisions[WallIndex].WallCollisionBox.BoxXRangeA && NextXRayPoint >= AllWallsCollisions[WallIndex].WallCollisionBox.BoxXRangeB && NextYRayPoint <= AllWallsCollisions[WallIndex].WallCollisionBox.BoxYRangeA && NextYRayPoint >= AllWallsCollisions[WallIndex].WallCollisionBox.BoxYRangeB && NextZRayPoint <= AllWallsCollisions[WallIndex].WallCollisionBox.BoxZRangeA && NextZRayPoint >= AllWallsCollisions[WallIndex].WallCollisionBox.BoxZRangeB)
            // bool collision = ((abs(NextXRayPoint - AllWallsCollisions[i2].xPos) < AllWallsCollisions[i2].WallCollisionBox.xSize) &&
            //				  (abs(NextYRayPoint - AllWallsCollisions[i2].yPos) < AllWallsCollisions[i2].WallCollisionBox.ySize) &&
            //				  (abs(NextZRayPoint - AllWallsCollisions[i2].zPos) < AllWallsCollisions[i2].WallCollisionBox.zSize));

            // if (collision)
            {
                // Set wall hit flash position
                float LastNextXRayPointFixed = LastNextXRayPoint / 8192.0;
                float LastNextYRayPointFixed = LastNextYRayPoint / 8192.0;
                float LastNextZRayPointFixed = LastNextZRayPoint / 8192.0;
                NE_ModelSetCoord(Model[8], LastNextXRayPointFixed, LastNextYRayPointFixed, LastNextZRayPointFixed);

                // Get wall hit flash rotation
                Vector2 Direction1D;
                Direction1D.y = LastNextYRayPointFixed - player->yPos - CameraOffsetY + y;
                Direction1D.x = 1;
                normalize2D(&Direction1D);

                Vector3 Direction;
                Direction.x = LastNextXRayPointFixed - player->xPos;
                Direction.y = LastNextYRayPointFixed - player->yPos;
                Direction.z = LastNextZRayPointFixed - player->zPos;
                normalize(&Direction);

                // Set wall hit flash angles
                int FinalAngleY = atan2f(Direction.x, Direction.z) * 512.0 / (M_TWOPI) + 384;
                int FinalAngleZ = atan2f(Direction1D.y, 1) * 512.0 / (M_TWOPI) + 128;
                NE_ModelSetRot(Model[8], 0, FinalAngleY, FinalAngleZ);

                ShowWallHitFlash = 3;
                // printf("HIT WALL\n");

                // Prepare wall hit data for server
                SendWallHit = true;
                WallHitXPos = LastNextXRayPoint;
                WallHitYPos = LastNextYRayPoint;
                WallHitZPos = LastNextZRayPoint;

                // TODO CHANGE THIS
                // if (player->AllGunsInInventory[player->currentGunInInventory] < GunCount && AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].isKnife) //Play knife sound it a wall was found
                // PlayBasicSound(AllGuns[player->AllGunsInInventory[player->currentGunInInventory]].gunSound);
                if (AllGuns[player->startGunIdRaycast].isKnife && !player->justCheking) // Play knife sound it a wall was found
                    PlayBasicSound(AllGuns[player->startGunIdRaycast].gunSound);
                return -1;
            }
        }
    }

    return 0;
}

float sign(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y)
{
    return (p1x - p3x) * (p2y - p3y) - (p2x - p3x) * (p1y - p3y);
}

bool PointInTriangle(float px, float py, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(px, py, p1x, p1y, p2x, p2y);
    d2 = sign(px, py, p2x, p2y, p3x, p3y);
    d3 = sign(px, py, p3x, p3y, p1x, p1y);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

int signInt(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y)
{
    return (p1x - p3x) * (p2y - p3y) - (p2x - p3x) * (p1y - p3y);
}

bool PointInTriangleInt(int px, int py, int p1x, int p1y, int p2x, int p2y, int p3x, int p3y)
{
    int d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(px, py, p1x, p1y, p2x, p2y);
    d2 = sign(px, py, p2x, p2y, p3x, p3y);
    d3 = sign(px, py, p3x, p3y, p1x, p1y);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}