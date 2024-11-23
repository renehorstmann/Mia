#include "m/sca/flt.h"
#include "m/io/vecn.h"
#include "o/OObjRoot.h"
#include "o/OJson.h"
#include "o/terminalcolor.h"
#include <locale.h>


osize vecn_buffer(char *str, osize size, const float *v, int n, bool colored, bool typed)
{
    setlocale(LC_ALL, "C");
    if (!str || !size) {
        str = NULL;
        size = 0;
    }
    osize used = 0;
    typed = typed && n >= 2 && n <= 4;

    if (typed) {
        used += snprintf(str, size, "(vec%i) ", n);
    } else {
        used += snprintf(str, size, "(float[%i]) ", n);
    }
    if(used>=size) {
        str = NULL;
    }

    if (colored) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         O_TERMINALCOLOR_CYAN);
        if(used>=size) {
            str = NULL;
        }
    }

    if (typed) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "{{ ");
    } else {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "{  ");
    }
    if(used>=size) {
        str = NULL;
    }

    for (int i = 0; i < n; i++) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "%" m_PRI "%s", v[i], i < n - 1 ? ", " : "");
        if (used >= size) {
            str = NULL;
        }
    }

    if (typed && n >= 2 && n <= 4) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used, " }}");
    } else {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used, "  }");
    }
    if (used >= size) {
        str = NULL;
    }


    if (colored) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         O_TERMINALCOLOR_RESET);
    }

    return used;
}

char *vecn_str(oobj parent, const float *v, int n, bool colored, bool typed)
{
    osize len = vecn_buffer(NULL, 0, v, n, colored, typed);
    len += 1;
    char *str = o_new(parent, char, len);
    vecn_buffer(str, len, v, n, colored, typed);
    return str;
}

osize matn_buffer(char *str, osize size, const float *m, int n, bool colored, bool typed, bool multiline)
{
    setlocale(LC_ALL, "C");
    if (!str || !size) {
        str = NULL;
        size = 0;
    }
    osize used = 0;
    typed = typed && n >= 2 && n <= 4;

    if (typed) {
        used += snprintf(str, size, "(mat%i) ", n);
    } else {
        used += snprintf(str, size, "(mat[%i]) ", n);
    }
    if(used>=size) {
        str = NULL;
    }

    if (colored) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         O_TERMINALCOLOR_CYAN);
        if(used>=size) {
            str = NULL;
        }
    }

    if(multiline) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "\n");
        if(used>=size) {
            str = NULL;
        }
    }

    if (typed) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "{{ ");
    } else {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         "{  ");
    }
    if(used>=size) {
        str = NULL;
    }

    for (int col = 0; col < n; col++) {
        for(int row=0; row < n; row++) {
            used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                             "%" m_PRI "%s", m[col * n + row], row < n - 1 ? ", " : "");
            if (used >= size) {
                str = NULL;
            }
        }
        if(col < n-1) {
            if(multiline) {
                used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                                 ",\n   ");
            } else {
                used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                                 ", /**/ ");
            }
            if(used>=size) {
                str = NULL;
            }
        }
    }

    if (typed && n >= 2 && n <= 4) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used, " }}");
    } else {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used, "  }");
    }
    if (used >= size) {
        str = NULL;
    }


    if (colored) {
        used += snprintf(!str ? NULL : str + used, !size ? 0 : size - used,
                         O_TERMINALCOLOR_RESET);
    }

    return used;
}

char *matn_str(oobj parent, const float *m, int n, bool colored, bool typed, bool multiline)
{
    osize len = matn_buffer(NULL, 0, m, n, colored, typed, multiline);
    len += 1;
    char *str = o_new(parent, char, len);
    matn_buffer(str, len, m, n, colored, typed, multiline);
    return str;
}

osize vecn_print(const float *v, int n)
{
    oobj root = OObjRoot_new_heap();
    char *str = vecn_str(root, v, n, true, true);
    o_terminalcolor_start();
    osize written = printf("%s", str);
    o_terminalcolor_stop();
    o_del(root);
    return written;
}

osize vecn_println(const float *v, int n)
{
    osize written = vecn_print(v, n);
    written += printf("\n");
    return written;
}

osize matn_print(const float *m, int n, bool multiline)
{
    oobj root = OObjRoot_new_heap();
    char *str = matn_str(root, m, n, true, true, multiline);
    o_terminalcolor_start();
    osize written = printf("%s", str);
    o_terminalcolor_stop();
    o_del(root);
    return written;
}

osize matn_println(const float *m, int n, bool multiline)
{
    osize written = matn_print(m, n, multiline);
    written += printf("\n");
    return written;
}

bool vecn_json_read(float *out_v, int n, oobj json_array)
{
    OJson **list = OJson_list(json_array, NULL);
    int idx = 0;
    for(OJson **it=list; *it && idx<n; it++) {
        double *num = OJson_number(*it);
        if(num) {
            out_v[idx++] = (float) *num;
        }
    }
    o_free(json_array, list);
    return idx==n;
}

oobj vecn_json_new(oobj parent, const char *name, const float *v, int n)
{
    OJson *array = OJson_new_array(parent, name);
    OJson_packed_set(array, true);
    for(int i=0; i<n; i++) {
        char buf[32];
        snprintf(buf, sizeof buf, "v%i", i);
        OJson_new_number(array, buf, (double) v[i]);
    }
    return array;
}

bool matn_json_read(float *out_m, int n, oobj json_array)
{
    OJson **list = OJson_list(json_array, NULL);
    int col_idx = 0;
    int idx = 0;
    for(OJson **it=list; *it && col_idx < n; it++) {
        if(OJson_type(*it) != OJson_TYPE_ARRAY) {
            continue;
        }

        OJson **vec = OJson_list(*it, NULL);
        int row_idx = 0;
        for(OJson **vec_it=vec; *vec_it && row_idx < n; vec_it++) {
            double *num = OJson_number(*vec_it);
            if(num) {
                out_m[idx++] = (float) *num;
                row_idx++;
            }
        }
        o_free(*it, vec);
        if(row_idx < n) {
            break;
        }
        col_idx++;
    }
    o_free(json_array, list);
    return idx==n*n;
}

oobj matn_json_new(oobj parent, const char *name, const float *m, int n)
{
    OJson *array = OJson_new_array(parent, name);
    for(int col=0; col<n; col++) {
        char buf[32];
        snprintf(buf, sizeof buf, "col[%i]", col);
        OJson *vec = OJson_new_array(array, name);
        OJson_packed_set(vec, true);
        for(int row=0; row<n; row++) {
            int idx = col*n + row;
            snprintf(buf, sizeof buf, "v%i", row);
            OJson_new_number(vec, buf, (double) m[idx]);
        }
    }
    return array;
}
