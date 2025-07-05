#pragma once
#include "Colors.h"
#include <string>
#include <vector>
#include <map>
#include "MoleculeParser.h"
#include "mStructure.h"

enum class BondDirection {
    Up,
    Down,
    Left,
    Right,
    UpLeft,
    UpRight,
    DownLeft,
    DownRight
};

BondDirection GetDirectionByIndex(int idx);

struct Bond {
    int from_id;
    int to_id;
    BondDirection direction;
    int bond_order;
};

struct Atom {
    int mol_id;
    std::string name;
    ColorName mol_color;
    double mol_size;
    float x, y, z;
    bool selected = false;
    std::map<BondDirection, int> bonds;
};

class ObjectContainer
{
private:
    int next_mol_id = 1;
    int next_bond_id = 1;

    std::vector<Atom> molecules;
    std::vector<Bond> bonds;

    ObjectContainer() = default;

public:
    static ObjectContainer& Get()
    {
        static ObjectContainer instance;
        return instance;
    }

    ObjectContainer(const ObjectContainer&) = delete;
    ObjectContainer& operator=(const ObjectContainer&) = delete;


    int AddAtomObject(const std::string& symbol, float x, float y, float z);

    int AddBond(int from_id, int to_id, BondDirection direction, int order = 1);

    // Á¶È¸
    Atom* GetAtomObject(int id);
    const Atom* GetAtomObject(int id) const;

    const std::vector<Atom>& GetAllMolecules() const { return molecules; }
    const std::vector<Bond>& GetAllBonds() const { return bonds; }


};
