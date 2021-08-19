/* Part of the generic incremental SAT API called 'ipasir'.
 *
 * This LICENSE applies to all software included in the IPASIR distribution,
 * except for those parts in sub-directories or in included software
 * distribution packages, such as tar and zip files, which have their own
 * license restrictions.  Those license restrictions are usually listed in the
 * corresponding LICENSE or COPYING files, either in the sub-directory or in
 * the included software distribution package (the tar or zip file).  Please
 * refer to those licenses for rights to use that software.
 *
 * Copyright (c) 2014, Tomas Balyo, Karlsruhe Institute of Technology.
 * Copyright (c) 2014, Armin Biere, Johannes Kepler University.
 * Copyright (c) 2015, Norbert Manthey
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */
#include "shadowipasir.h"
#include <ipasir.h>
#include <stdlib.h>        // getenv
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

// #ifdef __cplusplus
// extern "C" {
// #endif

std::string sig = std::string();

std::ofstream outfile;

std::vector<int> assumptions;

const char * ipasir_signature()
{
    if (sig.empty())
    {
        sig = std::string(shadow_ipasir_signature());
        sig.append(" with interception");
    }

    return sig.c_str();
}

void * ipasir_init()
{
    char tempfile[] = "/host/cnfXXXXXX";
    int fd = mkstemp(tempfile);
    close(fd);
    outfile = std::ofstream(tempfile);
    assumptions.clear();
    outfile << "p inccnf" << std::endl;
    return shadow_ipasir_init();
}

void ipasir_release(void * solver)
{
    if (outfile.is_open()) {
        outfile.close();
    }

    shadow_ipasir_release(solver);    
}

void ipasir_add(void * solver, int lit_or_zero)
{
    outfile << lit_or_zero;
    if (lit_or_zero == 0) {
        outfile << std::endl;
    }
    else {
        outfile << " ";
    }

    shadow_ipasir_add(solver, lit_or_zero);    
}

void ipasir_assume(void * solver, int lit)
{
    outfile << "c adding assumption: " << lit << std::endl;
    assumptions.push_back(lit);
    shadow_ipasir_assume(solver, lit);
}

int ipasir_solve(void * solver)
{
    outfile << "c Assumption Count: " << assumptions.size() << std::endl;
    outfile << "a";
    for(int val: assumptions) {
        outfile << " " << val; 
    }
    outfile << " 0" << std::endl;
    assumptions.clear();
    
    outfile << "c SOLVE REQUEST" << std::endl;
    int solve_val = shadow_ipasir_solve(solver);
    outfile << "c result: " << solve_val << std::endl;
    return solve_val;
}

int ipasir_val(void * solver, int lit)
{
   return shadow_ipasir_val(solver, lit);
}

int ipasir_failed(void * solver, int lit)
{
    return shadow_ipasir_failed(solver, lit);
}

void ipasir_set_terminate(void * solver, void * state, int (*terminate)(void * state))
{
    shadow_ipasir_set_terminate(solver, state, terminate);
}

void ipasir_set_learn(void * solver, void * state, int max_length, void (*learn)(void * state, int * clause))
{
    shadow_ipasir_set_learn(solver, state, max_length, learn);
}

// int main() {
//     printf("%s\n", ipasir_signature());
//     return 0;
// }