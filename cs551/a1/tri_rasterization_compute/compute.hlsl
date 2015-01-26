struct Vertex
{
    int2    position;
    float2  tex_coords;
};

struct Pixel
{
    int color; //assume 32bit rgba color format
};

cbuffer rasterizer_params : register(b0)
{
    int default_color;
    uint output_width, output_height;
}

StructuredBuffer<Vertex> input_vertices : register(b0);
RWStructuredBuffer<Pixel> output_texture : register(u0);

float3 barycentric(int2 pos, int2 v0, int2 v1, int2 v2)
{
        
}

int rasterize(int2 pos)
{
    
}

[numthreads(128, 128, 1)]
void CSMain( uint3 dispatch_tid : SV_DispatchThreadID )
{
        
}
