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
    float3 default_color, tri_color;
    int num_tris;
    uint output_width, output_height;
}

StructuredBuffer<Vertex> input_vertices : register(b0);
RWStructuredBuffer<Pixel> output_texture : register(u0);

//compute barycentric coords by solving system of eqns
//contact me for derivation if necessary
float3 barycentric(int2 pos, int2 a, int2 b, int2 c)
{
    float3 res; 

    float2 v0 =  pos - a;
    float2 v1 = b - a;
    float2 v2 = c - a;
    
    float d20 = dot(v2, v0);
    float d12 = dot(v1, v2);
    float d22 = dot(v2, v2);
    float d10 = dot(v1, v0);
    float d11 = dot(v1, v1);
    float d21 = dot(v2, v1);

    float denom = d22*d11 - d21*d12;

    res.y = (d10*d22 - d20*d21) / denom;
    res.z = (d20*d11 - d10*d12) / denom;
    res.x = 1.0f - (res.y + res.z);
    return res;
}

float3 rasterize(int2 pos, int2 vert0, int2 vert1, int2 vert2)
{
    res = barycentric(pos, vert0, vert1, vert2);
    
    if(res.x >= 0.0f && res.y >= 0.0f && res.z >= 0.0f)
        return default_color;
    else
        return tri_color;
}

[numthreads(128, 128, 1)]
void CSMain( uint3 dispatch_tid : SV_DispatchThreadID )
{
    uint index; 
    int output_red, output_green, output_blue, output_alpha;
    float3 pix_color;

    for(int i = 0; i < num_tris; i += 3)
    {
        //TODO: synchronize
        index = dispatch_tid.y * image_width + dispatch_tid.x;
        pix_color = rasterize(
                      int2(dispatch_tid.x, dispatch_tid.y),
                      input_vertices[i],
                      input_vertices[i+1],
                      input_vertices[i+2]);

        output_red =    (int)pix_color.x*255;
        output_green =  (int)(pix_color.y*255) << 8;
        output_blue =   (int)(pix_color.z*255) << 16;
        output_alpha =  (int)(255) << 24;
        
        output_texture[index] = output_red +
                                output_green +
                                output_blue +
                                output_alpha;

        //TODO: synchronize
    }
}
