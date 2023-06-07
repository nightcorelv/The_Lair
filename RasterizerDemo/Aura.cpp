#include "Aura.h"
#include "Player.h"
#include "stb_image.h"

Aura* Aura::currentAura;
Player* Aura::s_player;

Aura::PlayerPosConstantStruct Aura::playerS;
ID3D11Buffer* Aura::constantBuffer;
D3D11_MAPPED_SUBRESOURCE Aura::mappedResource{};

ID3D11BlendState* Aura::blendState;
ID3D11SamplerState* Aura::sampler;
std::unordered_map<std::string, ID3D11Texture2D*> Aura::textureMap;
std::unordered_map<std::string, ID3D11ShaderResourceView*> Aura::textureSRV;
std::string Aura::textureKey = "../Textures/auraParticleTex.png";
ID3D11DeviceContext* a_immediateContext;


Aura::Aura(ID3D11Device* device, XMFLOAT3 Position, XMFLOAT3 Rotation, int dmg, float radius)
	:Pickup(device, "../Meshes/scroll", Position), dmg(dmg), radius(radius)
{
    setID(ID::aura);
    currentAura = this;
}

Aura::Aura(ID3D11Device* device, LspMesh& mesh, XMFLOAT3 Position, XMFLOAT3 Rotation, int dmg, float radius)
    :Pickup(device, mesh, Position), dmg(dmg), radius(radius)
{
    setID(ID::aura);
    currentAura = this;
}

int Aura::getDmg() const
{
    return dmg;
}

float Aura::getRadius() const
{
    return radius;
}

void Aura::combineAuras(Aura* other)
{
    this->dmg += other->dmg;
}

bool Aura::collision(Entity& entity)
{
    return false;
}

void Aura::makeVirtual()
{
}

void Aura::damagecountdown()
{
    
    if (damageTimer >= damagetime)
    {
         setCanDamage(true);
        damageTimer = 0;
    }
    else
    {
        setCanDamage(false);
        damageTimer++;

    }
   
}

void Aura::setCanDamage(bool state)
{
    candamage = state;
}

bool Aura::canDamage()
{
    return candamage;
}

void Aura::setdamageTimer0()
{
    damageTimer =0;
}

void Aura::Reset()
{
    if (a_immediateContext != nullptr)
    {
        RtlZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
        a_immediateContext->Map(constantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

        playerS.pos[0] = 500;
        playerS.pos[1] = 500;
        playerS.pos[2] = 500;
        playerS.radios = currentAura->radius;

        memcpy(mappedResource.pData, &playerS, 16u);
        a_immediateContext->Unmap(constantBuffer, 0);

        a_immediateContext->PSSetSamplers(0, 1, &sampler);
        a_immediateContext->CSSetConstantBuffers(2, 1, &constantBuffer);
        a_immediateContext->PSSetShaderResources(0, 1, &textureSRV[textureKey]);

        a_immediateContext->PSSetSamplers(0, 0, nullptr);
        //immediateContext->OMSetBlendState(blendState, nullptr, 0xffffffffu);
    }

}

void Aura::Init(ID3D11Device* device)
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    device->CreateSamplerState(&samplerDesc, &sampler);

    D3D11_BLEND_DESC blendDesc{};
    D3D11_RENDER_TARGET_BLEND_DESC& brt = blendDesc.RenderTarget[0];
    brt.BlendEnable = true;
    brt.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
    brt.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
    brt.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    brt.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
    brt.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
    brt.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
    brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
    device->CreateBlendState(&blendDesc, &blendState);

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
    cbDesc.ByteWidth = 16u;
    cbDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA subData = {};
    subData.pSysMem = &playerS;
    device->CreateBuffer(&cbDesc, &subData, &constantBuffer);


    std::vector<std::string> texturePath{ textureKey,};

    for (std::string& path : texturePath)
    {

        int width, height, channels;
        unsigned char* color = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        ID3D11Texture2D* tex = nullptr;
        ID3D11ShaderResourceView* srv = nullptr;

        D3D11_TEXTURE2D_DESC texDesc{
            texDesc.Width = width,
            texDesc.Height = height,
            texDesc.MipLevels = 1,
            texDesc.ArraySize = 1,
            texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            texDesc.SampleDesc.Count = 1,
            texDesc.SampleDesc.Quality = 0,
            texDesc.Usage = D3D11_USAGE_IMMUTABLE,
            texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE,
            texDesc.CPUAccessFlags = 0,
            texDesc.MiscFlags = 0
        };
        D3D11_SUBRESOURCE_DATA subRD{
            subRD.pSysMem = color,
            subRD.SysMemPitch = width * 4,
            subRD.SysMemSlicePitch = 0
        };

        device->CreateTexture2D(&texDesc, &subRD, &tex);
        textureMap.insert(std::make_pair(path, tex));

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;

        device->CreateShaderResourceView(tex, &srvDesc, &srv);
        textureSRV.insert(std::make_pair(path, srv));

        playerS.pos[0] = 500;
        playerS.pos[1] = 500;
        playerS.pos[2] = 500;

        if (color != nullptr)
            delete[] color;
    }
}

void Aura::SetPlayer(Player*& p)
{
    s_player = p;
}

void Aura::Bind(ID3D11DeviceContext* immediateContext)
{
    if (a_immediateContext == nullptr)
        a_immediateContext = immediateContext;

    Reset();

    if (s_player->hasAura())
    {
#pragma region UpdateConstant

        RtlZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
        immediateContext->Map(constantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

        playerS.pos[0] = s_player->getPosition().x;
        playerS.pos[1] = s_player->getPosition().y;
        playerS.pos[2] = s_player->getPosition().z;
        playerS.radios = currentAura->radius;

        memcpy(mappedResource.pData, &playerS, 16u);
        immediateContext->Unmap(constantBuffer, 0);

#pragma endregion
        immediateContext->PSSetSamplers(0, 1, &sampler);
        immediateContext->CSSetConstantBuffers(2, 1, &constantBuffer);
        immediateContext->PSSetShaderResources(0, 1, &textureSRV[textureKey]);
        //immediateContext->OMSetBlendState(blendState, nullptr, 0xffffffffu);
    }

}

void Aura::UnBind(ID3D11DeviceContext* immediateContext)
{
    //if (s_player->hasAura())
    //{
        immediateContext->PSSetSamplers(0, 0, nullptr);
        //immediateContext->OMSetBlendState(nullptr, nullptr, 0xffffffffu);
    //}

}

void Aura::Release()
{
    if (blendState != nullptr)
        blendState->Release();
    if (constantBuffer != nullptr)
        constantBuffer->Release();
    if (sampler != nullptr)
        sampler->Release();

    for (auto& element :textureMap)
    {
        if (element.second != nullptr)
            element.second->Release();
    }
    for (auto& element : textureSRV)
    {
        if (element.second != nullptr)
            element.second->Release();
    }
}