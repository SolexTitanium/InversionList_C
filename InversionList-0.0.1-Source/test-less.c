#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./inversion-list.h"
#include "./inversion-list.inc"

int main(void) {
  inversion_list_init();
  {
    unsigned int a[] = {2, 3, 4, 8, 9, 10};
    unsigned int b[100];

    for (int i = 0; i != sizeof(b) / sizeof(*b); i++) {
      b[i] = i + 1;
    }

    InversionList *set_a = inversion_list_create(11, sizeof a / sizeof *a, a);
    InversionList *set_b = inversion_list_create(101, sizeof b / sizeof *b, b);

    assert(inversion_list_less(set_a, set_b));

    inversion_list_destroy(set_a);
    inversion_list_destroy(set_b);

    unsigned int c[] = {2, 3, 4, 5, 6, 7, 56, 57};
    unsigned d[48 + 45];

    int index = 0;
    for (int value = 1; index != 48; index++, value++) {
      d[index] = value;
    }
    for (int value = 55; index != 48 + 45; index++, value++) {
      d[index] = value;
    }

    InversionList *set_c = inversion_list_create(158, sizeof c / sizeof *c, c);
    InversionList *set_d = inversion_list_create(101, sizeof d / sizeof *d, d);

    assert(inversion_list_less(set_c, set_d));

    inversion_list_destroy(set_c);
    inversion_list_destroy(set_d);
  }
  {
    unsigned int a[] = {3, 4, 5, 8, 12, 13};
    unsigned int b[] = {3, 4};
    unsigned int c[] = {4, 5};
    unsigned int d[] = {4};
    unsigned int e[] = {3, 4, 12};

    unsigned int f[] = {1};
    unsigned int g[] = {1, 2, 3};
    unsigned int h[] = {5, 6};
    unsigned int i[] = {5, 6, 7, 8};
    unsigned int j[] = {8, 9, 12, 13};
    unsigned int k[] = {3, 4, 5, 17, 18};

    unsigned int z[] = {3, 4, 5, 8, 12, 13};

    InversionList *set_a_uint8 = inversion_list_create(100, sizeof a / sizeof *a, a);
    InversionList *set_a_uint16 = inversion_list_create(1000, sizeof a / sizeof *a, a);
    InversionList *set_a_uint32 = inversion_list_create(100000, sizeof a / sizeof *a, a);

    InversionList *set_z_uint8 = inversion_list_create(100, sizeof z / sizeof *z, z);
    InversionList *set_z_uint16 = inversion_list_create(1000, sizeof z / sizeof *z, z);
    InversionList *set_z_uint32 = inversion_list_create(100000, sizeof z / sizeof *z, z);
    
    // inversion_list_less is true

    InversionList *set_b_uint8 = inversion_list_create(100, sizeof b / sizeof *b, b);
    InversionList *set_b_uint16 = inversion_list_create(1000, sizeof b / sizeof *b, b);
    InversionList *set_b_uint32 = inversion_list_create(100000, sizeof b / sizeof *b, b);
    
    InversionList *set_c_uint8 = inversion_list_create(100, sizeof c / sizeof *c, c);
    InversionList *set_c_uint16 = inversion_list_create(1000, sizeof c / sizeof *c, c);
    InversionList *set_c_uint32 = inversion_list_create(100000, sizeof c / sizeof *c, c);
    
    InversionList *set_d_uint8 = inversion_list_create(100, sizeof d / sizeof *d, d);
    InversionList *set_d_uint16 = inversion_list_create(1000, sizeof d / sizeof *d, d);
    InversionList *set_d_uint32 = inversion_list_create(100000, sizeof d / sizeof *d, d);
    
    InversionList *set_e_uint8 = inversion_list_create(100, sizeof e / sizeof *e, e);
    InversionList *set_e_uint16 = inversion_list_create(1000, sizeof e / sizeof *e, e);
    InversionList *set_e_uint32 = inversion_list_create(100000, sizeof e / sizeof *e, e);

    // inversion_list_less is false

    InversionList *set_f_uint8 = inversion_list_create(100, sizeof f / sizeof *f, f);
    InversionList *set_f_uint16 = inversion_list_create(1000, sizeof f / sizeof *f, f);
    InversionList *set_f_uint32 = inversion_list_create(100000, sizeof f / sizeof *f, f);

    InversionList *set_g_uint8 = inversion_list_create(100, sizeof g / sizeof *g, g);
    InversionList *set_g_uint16 = inversion_list_create(1000, sizeof g / sizeof *g, g);
    InversionList *set_g_uint32 = inversion_list_create(100000, sizeof g / sizeof *g, g);

    InversionList *set_h_uint8 = inversion_list_create(100, sizeof h / sizeof *h, h);
    InversionList *set_h_uint16 = inversion_list_create(1000, sizeof h / sizeof *h, h);
    InversionList *set_h_uint32 = inversion_list_create(100000, sizeof h / sizeof *h, h);

    InversionList *set_i_uint8 = inversion_list_create(100, sizeof i / sizeof *i, i);
    InversionList *set_i_uint16 = inversion_list_create(1000, sizeof i / sizeof *i, i);
    InversionList *set_i_uint32 = inversion_list_create(100000, sizeof i / sizeof *i, i);

    InversionList *set_j_uint8 = inversion_list_create(100, sizeof j / sizeof *j, j);
    InversionList *set_j_uint16 = inversion_list_create(1000, sizeof j / sizeof *j, j);
    InversionList *set_j_uint32 = inversion_list_create(100000, sizeof j / sizeof *j, j);

    InversionList *set_k_uint8 = inversion_list_create(100, sizeof k / sizeof *k, k);
    InversionList *set_k_uint16 = inversion_list_create(1000, sizeof k / sizeof *k, k);
    InversionList *set_k_uint32 = inversion_list_create(100000, sizeof k / sizeof *k, k);

    // Exactly the same values

    assert(!inversion_list_less(set_z_uint8, set_a_uint8));
    assert(!inversion_list_less(set_z_uint8, set_a_uint16));
    assert(!inversion_list_less(set_z_uint8, set_a_uint32));
    assert(!inversion_list_less(set_z_uint16, set_a_uint8));
    assert(!inversion_list_less(set_z_uint16, set_a_uint16));
    assert(!inversion_list_less(set_z_uint16, set_a_uint32));
    assert(!inversion_list_less(set_z_uint32, set_a_uint8));
    assert(!inversion_list_less(set_z_uint32, set_a_uint16));
    assert(!inversion_list_less(set_z_uint32, set_a_uint32));
    
    // inversion_list_less is true

    assert(inversion_list_less(set_b_uint8, set_a_uint8));
    assert(inversion_list_less(set_b_uint8, set_a_uint16));
    assert(inversion_list_less(set_b_uint8, set_a_uint32));
    assert(inversion_list_less(set_b_uint16, set_a_uint8));
    assert(inversion_list_less(set_b_uint16, set_a_uint16));
    assert(inversion_list_less(set_b_uint16, set_a_uint32));
    assert(inversion_list_less(set_b_uint32, set_a_uint8));
    assert(inversion_list_less(set_b_uint32, set_a_uint16));
    assert(inversion_list_less(set_b_uint32, set_a_uint32));

    assert(inversion_list_less(set_c_uint8, set_a_uint8));
    assert(inversion_list_less(set_c_uint8, set_a_uint16));
    assert(inversion_list_less(set_c_uint8, set_a_uint32));
    assert(inversion_list_less(set_c_uint16, set_a_uint8));
    assert(inversion_list_less(set_c_uint16, set_a_uint16));
    assert(inversion_list_less(set_c_uint16, set_a_uint32));
    assert(inversion_list_less(set_c_uint32, set_a_uint8));
    assert(inversion_list_less(set_c_uint32, set_a_uint16));
    assert(inversion_list_less(set_c_uint32, set_a_uint32));

    assert(inversion_list_less(set_d_uint8, set_a_uint8));
    assert(inversion_list_less(set_d_uint8, set_a_uint16));
    assert(inversion_list_less(set_d_uint8, set_a_uint32));
    assert(inversion_list_less(set_d_uint16, set_a_uint8));
    assert(inversion_list_less(set_d_uint16, set_a_uint16));
    assert(inversion_list_less(set_d_uint16, set_a_uint32));
    assert(inversion_list_less(set_d_uint32, set_a_uint8));
    assert(inversion_list_less(set_d_uint32, set_a_uint16));
    assert(inversion_list_less(set_d_uint32, set_a_uint32));

    assert(inversion_list_less(set_e_uint8, set_a_uint8));
    assert(inversion_list_less(set_e_uint8, set_a_uint16));
    assert(inversion_list_less(set_e_uint8, set_a_uint32));
    assert(inversion_list_less(set_e_uint16, set_a_uint8));
    assert(inversion_list_less(set_e_uint16, set_a_uint16));
    assert(inversion_list_less(set_e_uint16, set_a_uint32));
    assert(inversion_list_less(set_e_uint32, set_a_uint8));
    assert(inversion_list_less(set_e_uint32, set_a_uint16));
    assert(inversion_list_less(set_e_uint32, set_a_uint32));

    // inversion_list_less is true

    assert(!inversion_list_less(set_f_uint8, set_a_uint8));
    assert(!inversion_list_less(set_f_uint8, set_a_uint16));
    assert(!inversion_list_less(set_f_uint8, set_a_uint32));
    assert(!inversion_list_less(set_f_uint16, set_a_uint8));
    assert(!inversion_list_less(set_f_uint16, set_a_uint16));
    assert(!inversion_list_less(set_f_uint16, set_a_uint32));
    assert(!inversion_list_less(set_f_uint32, set_a_uint8));
    assert(!inversion_list_less(set_f_uint32, set_a_uint16));
    assert(!inversion_list_less(set_f_uint32, set_a_uint32));

    assert(!inversion_list_less(set_g_uint8, set_a_uint8));
    assert(!inversion_list_less(set_g_uint8, set_a_uint16));
    assert(!inversion_list_less(set_g_uint8, set_a_uint32));
    assert(!inversion_list_less(set_g_uint16, set_a_uint8));
    assert(!inversion_list_less(set_g_uint16, set_a_uint16));
    assert(!inversion_list_less(set_g_uint16, set_a_uint32));
    assert(!inversion_list_less(set_g_uint32, set_a_uint8));
    assert(!inversion_list_less(set_g_uint32, set_a_uint16));
    assert(!inversion_list_less(set_g_uint32, set_a_uint32));

    assert(!inversion_list_less(set_h_uint8, set_a_uint8));
    assert(!inversion_list_less(set_h_uint8, set_a_uint16));
    assert(!inversion_list_less(set_h_uint8, set_a_uint32));
    assert(!inversion_list_less(set_h_uint16, set_a_uint8));
    assert(!inversion_list_less(set_h_uint16, set_a_uint16));
    assert(!inversion_list_less(set_h_uint16, set_a_uint32));
    assert(!inversion_list_less(set_h_uint32, set_a_uint8));
    assert(!inversion_list_less(set_h_uint32, set_a_uint16));
    assert(!inversion_list_less(set_h_uint32, set_a_uint32));

    assert(!inversion_list_less(set_i_uint8, set_a_uint8));
    assert(!inversion_list_less(set_i_uint8, set_a_uint16));
    assert(!inversion_list_less(set_i_uint8, set_a_uint32));
    assert(!inversion_list_less(set_i_uint16, set_a_uint8));
    assert(!inversion_list_less(set_i_uint16, set_a_uint16));
    assert(!inversion_list_less(set_i_uint16, set_a_uint32));
    assert(!inversion_list_less(set_i_uint32, set_a_uint8));
    assert(!inversion_list_less(set_i_uint32, set_a_uint16));
    assert(!inversion_list_less(set_i_uint32, set_a_uint32));

    assert(!inversion_list_less(set_j_uint8, set_a_uint8));
    assert(!inversion_list_less(set_j_uint8, set_a_uint16));
    assert(!inversion_list_less(set_j_uint8, set_a_uint32));
    assert(!inversion_list_less(set_j_uint16, set_a_uint8));
    assert(!inversion_list_less(set_j_uint16, set_a_uint16));
    assert(!inversion_list_less(set_j_uint16, set_a_uint32));
    assert(!inversion_list_less(set_j_uint32, set_a_uint8));
    assert(!inversion_list_less(set_j_uint32, set_a_uint16));
    assert(!inversion_list_less(set_j_uint32, set_a_uint32));

    assert(!inversion_list_less(set_k_uint8, set_a_uint8));
    assert(!inversion_list_less(set_k_uint8, set_a_uint16));
    assert(!inversion_list_less(set_k_uint8, set_a_uint32));
    assert(!inversion_list_less(set_k_uint16, set_a_uint8));
    assert(!inversion_list_less(set_k_uint16, set_a_uint16));
    assert(!inversion_list_less(set_k_uint16, set_a_uint32));
    assert(!inversion_list_less(set_k_uint32, set_a_uint8));
    assert(!inversion_list_less(set_k_uint32, set_a_uint16));
    assert(!inversion_list_less(set_k_uint32, set_a_uint32));

    inversion_list_destroy(set_a_uint8);
    inversion_list_destroy(set_a_uint16);
    inversion_list_destroy(set_a_uint32);
    inversion_list_destroy(set_b_uint8);
    inversion_list_destroy(set_b_uint16);
    inversion_list_destroy(set_b_uint32);
    inversion_list_destroy(set_c_uint8);
    inversion_list_destroy(set_c_uint16);
    inversion_list_destroy(set_c_uint32);
    inversion_list_destroy(set_d_uint8);
    inversion_list_destroy(set_d_uint16);
    inversion_list_destroy(set_d_uint32);
    inversion_list_destroy(set_e_uint8);
    inversion_list_destroy(set_e_uint16);
    inversion_list_destroy(set_e_uint32);
    inversion_list_destroy(set_f_uint8);
    inversion_list_destroy(set_f_uint16);
    inversion_list_destroy(set_f_uint32);
    inversion_list_destroy(set_g_uint8);
    inversion_list_destroy(set_g_uint16);
    inversion_list_destroy(set_g_uint32);
    inversion_list_destroy(set_h_uint8);
    inversion_list_destroy(set_h_uint16);
    inversion_list_destroy(set_h_uint32);
    inversion_list_destroy(set_i_uint8);
    inversion_list_destroy(set_i_uint16);
    inversion_list_destroy(set_i_uint32);
    inversion_list_destroy(set_j_uint8);
    inversion_list_destroy(set_j_uint16);
    inversion_list_destroy(set_j_uint32);
    inversion_list_destroy(set_k_uint8);
    inversion_list_destroy(set_k_uint16);
    inversion_list_destroy(set_k_uint32);

    inversion_list_destroy(set_z_uint8);
    inversion_list_destroy(set_z_uint16);
    inversion_list_destroy(set_z_uint32);
  }
  inversion_list_finish();
  return EXIT_SUCCESS;
}
