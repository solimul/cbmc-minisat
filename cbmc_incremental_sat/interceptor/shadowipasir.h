/* Part of the generic incremental SAT API called 'ipasir'.
 * See 'LICENSE' for rights to use this software.
 */
#ifndef shadowipasir_h_INCLUDED
#define shadowipasir_h_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

const char * shadow_ipasir_signature();

void * shadow_ipasir_init();

void shadow_ipasir_release(void * solver);

void shadow_ipasir_add(void * solver, int lit_or_zero);

void shadow_ipasir_assume(void * solver, int lit);

int shadow_ipasir_solve(void * solver);

int shadow_ipasir_val(void * solver, int lit);

int shadow_ipasir_failed(void * solver, int lit);

void shadow_ipasir_set_terminate(void * solver, void * state, int (*terminate)(void * state));

void shadow_ipasir_set_learn(void * solver, void * state, int max_length, void (*learn)(void * state, int * clause));

#ifdef __cplusplus
}
#endif


#endif
