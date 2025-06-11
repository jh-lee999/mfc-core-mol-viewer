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
    bool parsingBond = false;
    std::vector<int> mol_ids;

    // 방향을 순차적으로 가져오기 위한 벡터
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

    while (std::getline(file, line)) {
        if (line.empty()) {
            parsingBond = true;
            continue;
        }

        std::istringstream iss(line);

        if (!parsingBond) {
            float x, y, z;
            std::string name;
            iss >> x >> y >> z >> name;

            ColorName color = (name == "C") ? ColorName::Gray :
                (name == "O") ? ColorName::Red :
                (name == "H") ? ColorName::White : ColorName::White;

            int id = ObjectContainer::Get().AddAtomObject(name, color, 1.0, x, y, z);
            mol_ids.push_back(id);
        }
        else {
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
    }

    return true;
}
