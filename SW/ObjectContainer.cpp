#include "pch.h"
#include "ObjectContainer.h"

// �ݴ� ���� ���
static BondDirection GetOppositeDirection(BondDirection dir)
{
    switch (dir)
    {
    case BondDirection::Up: return BondDirection::Down;
    case BondDirection::Down: return BondDirection::Up;
    case BondDirection::Left: return BondDirection::Right;
    case BondDirection::Right: return BondDirection::Left;
    case BondDirection::UpLeft: return BondDirection::DownRight;
    case BondDirection::UpRight: return BondDirection::DownLeft;
    case BondDirection::DownLeft: return BondDirection::UpRight;
    case BondDirection::DownRight: return BondDirection::UpLeft;
    default: return BondDirection::Up;
    }
}

BondDirection GetDirectionByIndex(int idx)
{
    static std::vector<BondDirection> directions = {
        BondDirection::Up,
        BondDirection::Down,
        BondDirection::Left,
        BondDirection::Right,
        BondDirection::UpLeft,
        BondDirection::UpRight,
        BondDirection::DownLeft,
        BondDirection::DownRight
    };
    return directions[idx % directions.size()];
}

// ���� �߰�
int ObjectContainer::AddAtomObject(const std::string& symbol, float x, float y, float z)
{
    const AtomData* atomInfo = g_molDB.GetAtomData(symbol);
    if (!atomInfo) {
        AfxMessageBox(CString("�� �� ���� ����: ") + CString(symbol.c_str()));
        return -1;
    }

    Atom mol;
    mol.mol_id = next_mol_id++;
    mol.name = symbol;
    mol.mol_color = atomInfo->color;
    mol.mol_size = atomInfo->radius;
    mol.x = x;
    mol.y = y;
    mol.z = z;

    molecules.push_back(mol);
    return mol.mol_id;
}

// ���� �߰�
int ObjectContainer::AddBond(int from_id, int to_id, BondDirection direction, int order)
{
    Bond bond;
    bond.from_id = from_id;
    bond.to_id = to_id;
    bond.direction = direction;
    bond.bond_order = order;

    bonds.push_back(bond);

    Atom* from = GetAtomObject(from_id);
    Atom* to = GetAtomObject(to_id);
    if (from) from->bonds[direction] = to_id;
    if (to) to->bonds[GetOppositeDirection(direction)] = from_id;

    return static_cast<int>(bonds.size()); // bond_id�� ���� ������ �ε����� ��ü
}

// ���� ��ȸ (����)
Atom* ObjectContainer::GetAtomObject(int id)
{
    for (auto& m : molecules)
        if (m.mol_id == id) return &m;
    return nullptr;
}

// ���� ��ȸ (���)
const Atom* ObjectContainer::GetAtomObject(int id) const
{
    for (const auto& m : molecules)
        if (m.mol_id == id) return &m;
    return nullptr;
}

