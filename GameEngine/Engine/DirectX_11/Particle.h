#pragma once
#include"../GameObject/GameObject.h"
#include"BillBoard.h"
#include<list>
//�G�~�b�^�[�쐬���̃f�[�^
struct EmitterData
{
    std::string textureFileName;    //�\��e�N�X�`���̃t�@�C����
    XMFLOAT3 position;              //�G�~�b�^�[�̈ʒu
    XMFLOAT3 positionErr;           //�G�~�b�^�[�̌덷
    XMFLOAT3 dir;                   //�p�[�e�B�N���̔��˕���
    XMFLOAT3 dirErr;                //�p�[�e�B�N���̔��˕����̌덷
    float firstSpeed;                    //1�t���[��������̈ړ���
    float speedErr;                 //�ړ��ʂ̌덷
    float acceleration;             //�����x
    float gravity;                  //�d��
    XMFLOAT4 color;                 //�F : RGBA 0�`�P
    XMFLOAT4 deltaColor;            //�F�̕ω���
    XMFLOAT2 size;                  //�T�C�Y
    XMFLOAT2 sizeErr;               //�T�C�Y�̌덷
    XMFLOAT2 scale;                 //1�t���[��������̊g�嗦
    DWORD lifTime;                  //�p�[�e�B�N���̎���
    
    int delay;                      //�p�[�e�B�N�����o���Ԋu�̃t���[����
    int number;                     //1��ɏo���p�[�e�B�N���̐�

//Emitter������
        EmitterData()
        {
            textureFileName = "";
            position = positionErr = dir = dirErr = { 0,0,0 };
            firstSpeed = 0.0f;
            acceleration = 1.0f;
            gravity = 0.0f;
            color = { 1,1,1,1 };
            deltaColor = { 0,0,0,0 };
            size = scale = { 1.0f,1.0f };
            lifTime = 10;
        }

};
class Particle : public GameObject
{
private:
    //�G�~�b�^�[�̃f�[�^
    struct Emitter
    {
        EmitterData data;                   //�G�~�b�^�[�̃f�[�^
        int hParticle = -1;                 //�n���h��
        DWORD frameCount = 0;               //�o�߃t���[����
        BillBoard* pBillBoard = nullptr;    //�p�[�e�B�N���Ɏg���|���S��
        bool isDead = false;                //�������ǂ���
        int particleCount = 0;              //���̃G�~�b�^�[����o�č��c���Ă�p�[�e�B�N���̐�
    };

    //�p�[�e�B�N���̕ω�����f�[�^
    struct Data
    {
        XMFLOAT3 position;  //�ʒu
        XMFLOAT2 scale;     //�T�C�Y
        XMFLOAT4 color;     //�F
    };

    //�p�[�e�B�N���ꗱ�̃f�[�^
    struct ParticleData
    {
        Data     nowData;           //���̃p�[�e�B�N���f�[�^
        Data     deltaData;         //1�t���[���̕ω���
        DWORD    life;             //�c�����
        float    acceleration;     //�����x
        float    gravity;          //�d��
        Emitter* pEmitter;      //�G�~�b�^�[
    };

    std::list<Emitter*>      emitterList_;
    std::list<ParticleData*> particleList_;

public:
    Particle(GameObject* parent);

    //�f�X�g���N�^
    ~Particle();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�p�[�e�B�N���̃A�b�v�f�[�g
    void UpdateParticle();
    
    //�G�~�b�^�[�̃A�b�v�f�[�g
    void UpdateEmitter();

    //�p�[�e�B�N������
    int ParticleStart(EmitterData data);
    
    //�G�~�b�^�[����
    void KillEmitter(int hEmitter);

    void FixedUpdate() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};

