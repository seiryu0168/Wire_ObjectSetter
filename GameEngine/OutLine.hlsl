//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture		: register(t0);	//�e�N�X�`���[
Texture2D		g_normalTexture : register(t1); //�m�[�}���}�b�v
SamplerState	g_sampler		: register(s0);	//�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	float4x4	g_matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	g_matW;				//���[���h�s��
	float4x4    g_matNormal;		//�@���ό`�s��(��]�s��Ɗg��s��̋t�s��)
	float4		g_diffuseColor;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
	float4		g_ambient;			//�A���r�G���g
	float4		g_speculer;			//�X�y�L�����[
	float4		g_lightDirection;	//���C�g�̌���
	float4		g_cameraPosition;	//�J�����̈ʒu
	float4      g_customColor;		//�v���O�������ŐF��ς���ꍇ�̕ϐ�
	float		g_shininess;		//�n�C���C�g�̋���
	bool		g_isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
	bool		g_isNormal;  //�v���O�������ŕς���F
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos		: SV_POSITION;	//�ʒu
	float4 normal	: TEXCOORD0;	//�@��
	float4 light	: TEXCOORD1;	//���C�g
	float4 eyeVector: TEXCOORD2;	//����
	float4 col		: COLOR;		//�J���[
	float2 uv		: TEXCOORD3;	//UV���W
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData;
	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	//�����x�N�g��
	float4 cameraPos = mul(pos, g_matW);
	outData.eyeVector = normalize(g_cameraPosition - cameraPos);

	//�@��
	normal.w = 0;
	//outData.normal = mul(normal, g_matNormal);
	pos = pos + normal * 0.3f;
	outData.pos = mul(pos, g_matWVP);


	//�܂Ƃ߂ďo��
	return outData;
}
//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	return g_customColor;
}