#include "m/sca/flt.h"
#include "m/io/dvecn.h"
#include "o/OObjRoot.h"
#include "o/OJson.h"
#include "o/terminalcolor.h"
#include <locale.h>


osize dvecn_buffer(char *str, osize size, const double *v, int n, bool colored, bool typed)
{
    setlocale(LC_ALL, "C");
    if (!str || !size) {
        str = NULL;
        size = 0;
    }
    osize used = 0;
    typed = typed && n >= 2 && n <= 4;

    if (typed) {
        used += snprintf(str, size, "(dvec%i) ", n);
    } else {
        used += snprintf(str, size, "(double[%i]) ", n);
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

char *dvecn_str(oobj parent, const double *v, int n, bool colored, bool typed)
{
    osize len = dvecn_buffer(NULL, 0, v, n, colored, typed);
    len += 1;
    char *str = o_new(parent, char, len);
    dvecn_buffer(str, len, v, n, colored, typed);
    return str;
}

osize dmatn_buffer(char *str, osize size, const double *m, int n, bool colored, bool typed, bool multiline)
{
    setlocale(LC_ALL, "C");
    if (!str || !size) {
        str = NULL;
        size = 0;
    }
    osize used = 0;
    typed = typed && n >= 2 && n <= 4;

    if (typed) {
        used += snprintf(str, size, "(dmat%i) ", n);
    } else {
        used += snprintf(str, size, "(dmat[%i]) ", n);
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

char *dmatn_str(oobj parent, const double *m, int n, bool colored, bool typed, bool multiline)
{
    osize len = dmatn_buffer(NULL, 0, m, n, colored, typed, multiline);
    len += 1;
    char *str = o_new(parent, char, len);
    dmatn_buffer(str, len, m, n, colored, typed, multiline);
    return str;
}

osize dvecn_print(const double *v, int n)
{
    oobj root = OObjRoot_new_heap();
    char *str = dvecn_str(root, v, n, true, true);
    o_terminalcolor_start();
    osize written = printf("%s", str);
    o_terminalcolor_stop();
    o_del(root);
    return written;
}

osize dvecn_println(const double *v, int n)
{
    osize written = dvecn_print(v, n);
    written += printf("\n");
    return written;
}

osize dmatn_print(const double *m, int n, bool multiline)
{
    oobj root = OObjRoot_new_heap();
    char *str = dmatn_str(root, m, n, true, true, multiline);
    o_terminalcolor_start();
    osize written = printf("%s", str);
    o_terminalcolor_stop();
    o_del(root);
    return written;
}

osize dmatn_println(const double *m, int n, bool multiline)
{
    osize written = dmatn_print(m, n, multiline);
    written += printf("\n");
    return written;
}

bool dvecn_json_read(double *out_v, int n, oobj json_array)
{
    OJson **list = OJson_list(json_array, NULL);
    int idx = 0;
    for(OJson **it=list; *it && idx<n; it++) {
        double *num = OJson_number(*it);
        if(num) {
            out_v[idx++] = (double) *num;
        }
    }
    o_free(json_array, list);
    return idx==n;
}

oobj dvecn_json_new(oobj parent, const char *name, const double *v, int n)
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

bool dmatn_json_read(double *out_m, int n, oobj json_array)
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
                out_m[idx++] = (double) *num;
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

oobj dmatn_json_new(oobj parent, const char *name, const double *m, int n)
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
