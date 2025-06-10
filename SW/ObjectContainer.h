#pragma once
#include "Colors.h"
#include <string>
#include <vector>
#include <map>

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

struct Bond {
    int from_id;
    int to_id;
    BondDirection direction;
    int bond_order;
};

struct Molcule {
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

    std::vector<Molcule> molecules;
    std::vector<Bond> bonds;

    // ������ - �ܺο��� ���� ����
    ObjectContainer() = default;

public:
    // �̱��� �ν��Ͻ� ������
    static ObjectContainer& Get()
    {
        static ObjectContainer instance;
        return instance;
    }

    // ����/���� ����
    ObjectContainer(const ObjectContainer&) = delete;
    ObjectContainer& operator=(const ObjectContainer&) = delete;

    // ���� �߰�
    int AddMolecule(const std::string& name, ColorName color, double size, float x, float y, float z);

    // ���� �߰�
    int AddBond(int from_id, int to_id, BondDirection direction, int order = 1);

    // ��ȸ
    Molcule* GetMolecule(int id);
    const Molcule* GetMolecule(int id) const;

    const std::vector<Molcule>& GetAllMolecules() const { return molecules; }
    const std::vector<Bond>& GetAllBonds() const { return bonds; }

    // OpenGL ��������
    void DrawAll();
};
