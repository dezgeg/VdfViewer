#include <stdint.h>
#include <stdio.h>

#define D(v) printf(#v ": 0x%08x ", v)
#define nl() puts("")
#define where() printf("At: 0x%08x\n", 4 * (unsigned)(ptr - buf))

#define getvert(x) (*((uint16_t*)(((uint8_t*)ptr) + x)))
#define FACE(a, b, c, d) printf(d < 0 ? "TRI: %d %d %d\n" : "QUAD: %d %d %d %d\n", \
        getvert(a), getvert(b), getvert(c), getvert(d))

#define fixp(a) (((int16_t)a)/4096.)

#define VERT(x, y, z) printf("VERT %d: %f %f %f\n", vertIx++, fixp(x), fixp(y), fixp(z))

int main(int argc, char** argv)
{
    if (argc < 2)
        return 1;
    FILE* model = fopen(argv[1], "r");
    if (!model)
        return 2;

    uint32_t buf[20000];
    fread(&buf, sizeof(buf), 1, model);

    uint32_t* ptr = buf;
    // header
    uint32_t id = *ptr++;
    uint32_t flags = *ptr++;
    uint32_t nobj = *ptr++;

    D(id); D(flags); D(nobj); nl();
    uint8_t* objStart = (uint8_t*)ptr;

    for(int i = 0; i < nobj; i++) {
        printf("object %d:\n", i);

        uint32_t vert_top = *ptr++;
        uint32_t n_vert = *ptr++;
        uint32_t normal_top = *ptr++;
        uint32_t n_normal = *ptr++;
        uint32_t primitive_top = *ptr++;
        uint32_t n_primitive = *ptr++;
        uint32_t scale = *ptr++;
        D(vert_top); D(n_vert); nl();
        D(normal_top); D(n_normal); nl();
        D(primitive_top); D(n_primitive); nl();
        D(scale); nl();
        nl();


        uint32_t* savedPtr = ptr;
        // vertex dump
        int vertIx = 0;
        ptr = (uint32_t*)(objStart + vert_top);

        for (int j = 0; j < n_vert; j++) {
            uint16_t* hptr = (uint16_t*)ptr;
            uint32_t vx = *hptr++;
            uint32_t vy = *hptr++;
            uint32_t vz = *hptr++;
            VERT(vx, vy, vz);
            ptr += 2;
        }
        // primitive dump
        int faceIndex = 0;
        ptr = (uint32_t*)(objStart + primitive_top);

        for (int j = 0; j < n_primitive; j++) {
            uint8_t* bptr = (uint8_t*)ptr;
            uint32_t olen = *bptr++;
            uint32_t ilen = *bptr++;
            uint32_t flag = *bptr++;
            uint32_t mode = *bptr++;
            ptr++;

            D(mode); D(flag); D(ilen); D(olen); nl();
            const char* const lookup[] = { "???", "poly", "line", "sprite" };
            printf("%s: TGE:%d ABE:%d TME:%d 3/4:%d IIP:%d\n",
                    lookup[mode >> 5], !!(mode & 0x1), !!(mode & 0x2),
                    !!(mode & 0x4), !!(mode & 0x8), !!(mode & 0x10));

            switch (mode) {
                case 0x20: // tri, no texture
                    FACE(6, 8, 10, -1);
                    break;
                case 0x24: // tri, with texture
                    FACE(14, 16, 18, -1);
                    break;
                case 0x28: // quad, gradiation
                    FACE(18, 20, 22, 24);
                    break;
                case 0x2c: // quad, texture
                    FACE(18, 20, 22, 24);
                    break;
                case 0x30: // tri, gouraud
                    FACE(6, 10, 14, -1);
                    break;
                case 0x38: // quad, gouraud
                    FACE(6, 10, 14, 18);
                    break;

                default:
                    return 3;
            }

            ptr += ilen;
        }


        nl();
        ptr = savedPtr;
        break;
    }

    where();
}
