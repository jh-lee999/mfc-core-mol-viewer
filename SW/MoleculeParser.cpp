#include "pch.h"
#include "MoleculeParser.h"
#include "ObjectContainer.h"
#include <fstream>
#include <sstream>

bool MoleculeParser::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    std::vector<int> mol_ids;

    std::vector<BondDirection> directions = {
        BondDirection::Up,
        BondDirection::Down,
        BondDirection::Left,
        BondDirection::Right,
        BondDirection::UpLeft,
        BondDirection::UpRight,
        BondDirection::DownLeft,
        BondDirection::DownRight
    };

    int directionIndex = 0;

    // Skip header lines
    for (int i = 0; i < 3; ++i) std::getline(file, line);

    int atomCount = 0, bondCount = 0;
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> atomCount >> bondCount;
    }


    for (int i = 0; i < atomCount; ++i) {
        if (!std::getline(file, line)) break;
        std::istringstream iss(line);
        float x, y, z;
        std::string name;
        iss >> x >> y >> z >> name;


        int id = ObjectContainer::Get().FindAtom(name, x, y, z);
        if (id == -1) {
            id = ObjectContainer::Get().AddAtomObject(name, x, y, z);
        }

        mol_ids.push_back(id);
    }


    for (int i = 0; i < bondCount; ++i) {
        if (!std::getline(file, line)) break;
        std::istringstream iss(line);
        int fromIdx, toIdx, order;
        iss >> fromIdx >> toIdx >> order;

        if (fromIdx <= 0 || toIdx <= 0 ||
            fromIdx > mol_ids.size() || toIdx > mol_ids.size())
            continue;

        int from_id = mol_ids[fromIdx - 1];
        int to_id = mol_ids[toIdx - 1];

        BondDirection dir = directions[directionIndex++ % directions.size()];
        ObjectContainer::Get().AddBond(from_id, to_id, dir, order);
    }

    return true;
}

