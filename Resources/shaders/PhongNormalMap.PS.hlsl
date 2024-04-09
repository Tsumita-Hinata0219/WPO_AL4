#include "PhongNormalMap.hlsli"


struct Material
{
    float4 color;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct DirectionalLight
{
    float4 color;
    float3 direction;
    //float3 SpecularFColor;
    float intensity;
    float shininess;
    bool enableLighting;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<TransformationViewMatrix> gTransformationViewMatrix : register(b2);


Texture2D<float4> gTexture : register(t0);
Texture2D<float4> gTextureNormal : register(t1);
SamplerState gSampler : register(s0);

struct PixcelShaderOutput
{
    float4 color : SV_TARGET0;
};

static float3 NormalMap;

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float4 textureColorNormal = gTextureNormal.Sample(gSampler, input.texcoord);\

    NormalMap = normalize(input.normal + textureColorNormal.rgb);
    
    if (gDirectionalLight.enableLighting != 0)
    {
        float NdotL = dot(normalize(NormalMap), normalize(gDirectionalLight.direction));
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float3 toEye = normalize(input.cameraPosition - input.worldPosition);
        float3 reflectLight = reflect(normalize(gDirectionalLight.direction), normalize(NormalMap));
        float3 halfVector = normalize(gDirectionalLight.direction + toEye);
        float NdotH = dot(normalize(NormalMap), halfVector);
        float3 specularPow = pow(saturate(NdotH), gDirectionalLight.shininess);
        
        if (textureColor.a == 0.0f)
        {
            discard;
        }
        
        // �g�U����
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        // ���ʔ���
        float3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);
        
        // �g�U���� + ���ʔ���
        float3 AddiffuseSpecular = diffuse + specular;
        output.color.rgb = AddiffuseSpecular;
       
        // alpha�͍��܂Œʂ�
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
         
        if (textureColor.a == 0.0f)
        {
            discard;
        }
         
        output.color = gMaterial.color * textureColor;
    }

    return output;
}