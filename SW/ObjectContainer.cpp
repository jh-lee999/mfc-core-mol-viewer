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

// ���� �߰�
int ObjectContainer::AddMolecule(const std::string& name, ColorName color, double size, float x, float y, float z)
{
    Molcule mol;
    mol.mol_id = next_mol_id++;
    mol.name = name;
    mol.mol_color = color;
    mol.mol_size = size;
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

    Molcule* from = GetMolecule(from_id);
    Molcule* to = GetMolecule(to_id);
    if (from) from->bonds[direction] = to_id;
    if (to) to->bonds[GetOppositeDirection(direction)] = from_id;

    return static_cast<int>(bonds.size()); // bond_id�� ���� ������ �ε����� ��ü
}

// ���� ��ȸ (����)
Molcule* ObjectContainer::GetMolecule(int id)
{
    for (auto& m : molecules)
        if (m.mol_id == id) return &m;
    return nullptr;
}

// ���� ��ȸ (���)
const Molcule* ObjectContainer::GetMolecule(int id) const
{
    for (const auto& m : molecules)
        if (m.mol_id == id) return &m;
    return nullptr;
}

// �׸��� ���� �Լ�
void ObjectContainer::DrawAll()
{
    for (const auto& mol : molecules)
    {
    }

    for (const auto& bond : bonds)
    {
    }
}
