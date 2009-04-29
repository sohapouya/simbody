/* -------------------------------------------------------------------------- *
 *                      SimTK Core: SimTKcommon                               *
 * -------------------------------------------------------------------------- *
 * This is part of the SimTK Core biosimulation toolkit originating from      *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2008 Stanford University and the Authors.           *
 * Authors: Michael Sherman                                                   *
 * Contributors:                                                              *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR      *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 * USE OR OTHER DEALINGS IN THE SOFTWARE.                                     *
 * -------------------------------------------------------------------------- */

#include "SimTKcommon.h"
#include "SimTKcommon/Testing.h"

#include <iostream>
using std::cout;
using std::endl;

using namespace SimTK;


void testIsNaN() {
    const float  fltRegular = -12.34f;
    const double dblRegular = -12.34;
    const float fltNaN = NTraits<float>::getNaN();
    const double dblNaN = NTraits<double>::getNaN();
    const float nfltNaN = -fltNaN;
    const double ndblNaN = -dblNaN;

    SimTK_TEST(isNaN(fltNaN) && isNaN(dblNaN));
    SimTK_TEST(isNaN(nfltNaN) && isNaN(ndblNaN));
    SimTK_TEST(!isNaN(fltRegular) && !isNaN(dblRegular));

    std::complex<float> cflt(fltRegular, -2*fltRegular);
    std::complex<double> cdbl(dblRegular, -2*dblRegular);
    conjugate<float> cjflt(fltRegular, -2*fltRegular);
    conjugate<double> cjdbl(dblRegular, -2*dblRegular);

    SimTK_TEST(!isNaN(cflt) && !isNaN(cdbl));
    SimTK_TEST(!isNaN(cjflt) && !isNaN(cjdbl));

    // Reference the same memory as a negator of its contents.
    const negator<float>&           nflt   = reinterpret_cast<const negator<float>&>(fltRegular);
    const negator<double>&          ndbl   = reinterpret_cast<const negator<double>&>(dblRegular);
    negator<std::complex<float> >&   ncflt  = reinterpret_cast<negator<std::complex<float> >&> (cflt);
    negator<std::complex<double> >&  ncdbl  = reinterpret_cast<negator<std::complex<double> >&>(cdbl);
    negator<conjugate<float> >&      ncjflt = reinterpret_cast<negator<conjugate<float> >&>    (cjflt);
    negator<conjugate<double> >&     ncjdbl = reinterpret_cast<negator<conjugate<double> >&>   (cjdbl);

    // Test that negators are working properly.
    SimTK_TEST_NUMEQ(nflt, -fltRegular);
    SimTK_TEST_NUMEQ(ndbl, -dblRegular);
    SimTK_TEST_NUMEQ(ncflt, -cflt);
    SimTK_TEST_NUMEQ(-ncflt, cflt);
    SimTK_TEST_NUMEQ(ncjflt, -cjflt);
    SimTK_TEST_NUMEQ(-ncjflt, cjflt);

    SimTK_TEST(!isNaN(nflt) && !isNaN(ndbl));
    SimTK_TEST(!isNaN(ncflt) && !isNaN(ncdbl));
    SimTK_TEST(!isNaN(ncjflt) && !isNaN(ncjdbl));

    // Should be NaN if either or both parts are NaN.
    cflt = std::complex<float>(cflt.real(), fltNaN);
    cdbl = std::complex<double>(cdbl.real(), dblNaN);
    cjflt = conjugate<float>(cjflt.real(), fltNaN);
    cjdbl = conjugate<double>(cjdbl.real(), dblNaN);

    // Imaginary only is NaN.
    SimTK_TEST(isNaN(cflt) && isNaN(cdbl));
    SimTK_TEST(isNaN(cjflt) && isNaN(cjdbl));
    SimTK_TEST(isNaN(ncflt) && isNaN(ncdbl));
    SimTK_TEST(isNaN(ncjflt) && isNaN(ncjdbl));

    cflt = std::complex<float>(fltNaN, cflt.imag());
    cdbl = std::complex<double>(dblNaN, cdbl.imag());
    cjflt = conjugate<float>(fltNaN, cjflt.imag());
    cjdbl = conjugate<double>(dblNaN, cjdbl.imag());

    // Both parts are NaN.
    SimTK_TEST(isNaN(cflt) && isNaN(cdbl));
    SimTK_TEST(isNaN(cjflt) && isNaN(cjdbl));
    SimTK_TEST(isNaN(ncflt) && isNaN(ncdbl));
    SimTK_TEST(isNaN(ncjflt) && isNaN(ncjdbl));

    // Restore imaginary part to normal.
    cflt = std::complex<float>(cflt.real(), fltRegular);
    cdbl = std::complex<double>(cdbl.real(), dblRegular);
    cjflt = conjugate<float>(cjflt.real(), fltRegular);
    cjdbl = conjugate<double>(cjdbl.real(), dblRegular);

    // Real part only is NaN;
    SimTK_TEST(isNaN(cflt) && isNaN(cdbl));
    SimTK_TEST(isNaN(cjflt) && isNaN(cjdbl));
    SimTK_TEST(isNaN(ncflt) && isNaN(ncdbl));
    SimTK_TEST(isNaN(ncjflt) && isNaN(ncjdbl));
}

void testIsInf() {
    const float  fltRegular = -12.34f;
    const double dblRegular = -12.34;
    const float fltInf = NTraits<float>::getInfinity();
    const double dblInf = NTraits<double>::getInfinity();
    const float mfltInf = -fltInf;
    const double mdblInf = -dblInf;
    const negator<float>& nfltInf = reinterpret_cast<const negator<float>&>(fltInf);
    const negator<double>& ndblInf = reinterpret_cast<const negator<double>&>(dblInf);

    SimTK_TEST(nfltInf == -fltInf);
    SimTK_TEST(ndblInf == -dblInf);

    SimTK_TEST(isInf(fltInf) && isInf(dblInf));
    SimTK_TEST(isInf(mfltInf) && isInf(mdblInf));
    SimTK_TEST(isInf(nfltInf) && isInf(ndblInf));
    SimTK_TEST(!isInf(fltRegular) && !isInf(dblRegular));

    std::complex<float> cflt(fltRegular, -2*fltRegular);
    std::complex<double> cdbl(dblRegular, -2*dblRegular);
    conjugate<float> cjflt(fltRegular, -2*fltRegular);
    conjugate<double> cjdbl(dblRegular, -2*dblRegular);

    SimTK_TEST(!isInf(cflt) && !isInf(cdbl));
    SimTK_TEST(!isInf(cjflt) && !isInf(cjdbl));

    // Reference the same memory as a negator of its contents.
    const negator<float>&           nflt   = reinterpret_cast<const negator<float>&>(fltRegular);
    const negator<double>&          ndbl   = reinterpret_cast<const negator<double>&>(dblRegular);
    negator<std::complex<float> >&   ncflt  = reinterpret_cast<negator<std::complex<float> >&> (cflt);
    negator<std::complex<double> >&  ncdbl  = reinterpret_cast<negator<std::complex<double> >&>(cdbl);
    negator<conjugate<float> >&      ncjflt = reinterpret_cast<negator<conjugate<float> >&>    (cjflt);
    negator<conjugate<double> >&     ncjdbl = reinterpret_cast<negator<conjugate<double> >&>   (cjdbl);

    // Test that negators are working properly.
    SimTK_TEST_NUMEQ(nflt, -fltRegular);
    SimTK_TEST_NUMEQ(ndbl, -dblRegular);
    SimTK_TEST_NUMEQ(ncflt, -cflt);
    SimTK_TEST_NUMEQ(-ncflt, cflt);
    SimTK_TEST_NUMEQ(ncjflt, -cjflt);
    SimTK_TEST_NUMEQ(-ncjflt, cjflt);

    SimTK_TEST(!isInf(nflt) && !isInf(ndbl));
    SimTK_TEST(!isInf(ncflt) && !isInf(ncdbl));
    SimTK_TEST(!isInf(ncjflt) && !isInf(ncjdbl));

    // Should be Inf if either or both parts are Inf, as long as neither
    // part is NaN.
    cflt = std::complex<float>(cflt.real(), fltInf);
    cdbl = std::complex<double>(cdbl.real(), dblInf);
    cjflt = conjugate<float>(cjflt.real(), fltInf);
    cjdbl = conjugate<double>(cjdbl.real(), dblInf);

    // Imaginary only is Inf.
    SimTK_TEST(isInf(cflt) && isInf(cdbl));
    SimTK_TEST(isInf(cjflt) && isInf(cjdbl));
    SimTK_TEST(isInf(ncflt) && isInf(ncdbl));
    SimTK_TEST(isInf(ncjflt) && isInf(ncjdbl));

    cflt = std::complex<float>(fltInf, cflt.imag());
    cdbl = std::complex<double>(dblInf, cdbl.imag());
    cjflt = conjugate<float>(fltInf, cjflt.imag());
    cjdbl = conjugate<double>(dblInf, cjdbl.imag());

    // Both parts are Inf.
    SimTK_TEST(isInf(cflt) && isInf(cdbl));
    SimTK_TEST(isInf(cjflt) && isInf(cjdbl));
    SimTK_TEST(isInf(ncflt) && isInf(ncdbl));
    SimTK_TEST(isInf(ncjflt) && isInf(ncjdbl));

    // Restore imaginary part to normal.
    cflt = std::complex<float>(cflt.real(), fltRegular);
    cdbl = std::complex<double>(cdbl.real(), dblRegular);
    cjflt = conjugate<float>(cjflt.real(), fltRegular);
    cjdbl = conjugate<double>(cjdbl.real(), dblRegular);

    // Real part only is Inf;
    SimTK_TEST(isInf(cflt) && isInf(cdbl));
    SimTK_TEST(isInf(cjflt) && isInf(cjdbl));
    SimTK_TEST(isInf(ncflt) && isInf(ncdbl));
    SimTK_TEST(isInf(ncjflt) && isInf(ncjdbl));

    // Set real part to minus infinity.
    cflt = std::complex<float>(mfltInf, cflt.imag());
    cdbl = std::complex<double>(mdblInf, cdbl.imag());
    cjflt = conjugate<float>(mfltInf, cjflt.imag());
    cjdbl = conjugate<double>(mdblInf, cjdbl.imag());

    SimTK_TEST(isInf(cflt) && isInf(cdbl));
    SimTK_TEST(isInf(cjflt) && isInf(cjdbl));
    SimTK_TEST(isInf(ncflt) && isInf(ncdbl));
    SimTK_TEST(isInf(ncjflt) && isInf(ncjdbl));

    // Set real part to NaN.
    const float fltNaN = NTraits<float>::getNaN();
    const double dblNaN = NTraits<double>::getNaN();
    cflt = std::complex<float>(fltNaN, cflt.imag());
    cdbl = std::complex<double>(dblNaN, cdbl.imag());
    cjflt = conjugate<float>(fltNaN, cjflt.imag());
    cjdbl = conjugate<double>(dblNaN, cjdbl.imag());

    SimTK_TEST(!isInf(cflt) && !isInf(cdbl));
    SimTK_TEST(!isInf(cjflt) && !isInf(cjdbl));
    SimTK_TEST(!isInf(ncflt) && !isInf(ncdbl));
    SimTK_TEST(!isInf(ncjflt) && !isInf(ncjdbl));
}

void testIsFinite() {
    const float  fltRegular = -12.34f;
    const double dblRegular = -12.34;
    const float fltNaN = NTraits<float>::getNaN();
    const double dblNaN = NTraits<double>::getNaN();
    const float nfltNaN = -fltNaN;
    const double ndblNaN = -dblNaN;
    const float fltInf = NTraits<float>::getInfinity();
    const double dblInf = NTraits<double>::getInfinity();
    const float mfltInf = -fltInf;
    const double mdblInf = -dblInf;

    SimTK_TEST(isFinite(fltRegular) && isFinite(dblRegular));
    SimTK_TEST(!isFinite(fltNaN) && !isFinite(dblNaN));
    SimTK_TEST(!isFinite(fltInf) && !isFinite(dblInf));
    SimTK_TEST(!isFinite(mfltInf) && !isFinite(mdblInf));

    std::complex<float> cflt(fltRegular, -2*fltRegular);
    std::complex<double> cdbl(dblRegular, -2*dblRegular);
    conjugate<float> cjflt(fltRegular, -2*fltRegular);
    conjugate<double> cjdbl(dblRegular, -2*dblRegular);

    SimTK_TEST(isFinite(cflt) && isFinite(cdbl));
    SimTK_TEST(isFinite(cjflt) && isFinite(cjdbl));

    // Reference the same memory as a negator of its contents.
    const negator<float>&           nflt   = reinterpret_cast<const negator<float>&>(fltRegular);
    const negator<double>&          ndbl   = reinterpret_cast<const negator<double>&>(dblRegular);
    negator<std::complex<float> >&   ncflt  = reinterpret_cast<negator<std::complex<float> >&> (cflt);
    negator<std::complex<double> >&  ncdbl  = reinterpret_cast<negator<std::complex<double> >&>(cdbl);
    negator<conjugate<float> >&      ncjflt = reinterpret_cast<negator<conjugate<float> >&>    (cjflt);
    negator<conjugate<double> >&     ncjdbl = reinterpret_cast<negator<conjugate<double> >&>   (cjdbl);

    // Test that negators are working properly.
    SimTK_TEST_NUMEQ(nflt, -fltRegular);
    SimTK_TEST_NUMEQ(ndbl, -dblRegular);
    SimTK_TEST_NUMEQ(ncflt, -cflt);
    SimTK_TEST_NUMEQ(-ncflt, cflt);
    SimTK_TEST_NUMEQ(ncjflt, -cjflt);
    SimTK_TEST_NUMEQ(-ncjflt, cjflt);

    SimTK_TEST(isFinite(nflt) && isFinite(ndbl));
    SimTK_TEST(isFinite(ncflt) && isFinite(ncdbl));
    SimTK_TEST(isFinite(ncjflt) && isFinite(ncjdbl));

    // Should be finite only if both parts are finite.
    cflt = std::complex<float>(cflt.real(),  fltInf);
    cdbl = std::complex<double>(cdbl.real(), mdblInf);
    cjflt = conjugate<float>(cjflt.real(),   fltNaN);
    cjdbl = conjugate<double>(cjdbl.real(),  dblInf);

    // Imaginary only is NaN.
    SimTK_TEST(!isFinite(cflt) && !isFinite(cdbl));
    SimTK_TEST(!isFinite(cjflt) && !isFinite(cjdbl));
    SimTK_TEST(!isFinite(ncflt) && !isFinite(ncdbl));
    SimTK_TEST(!isFinite(ncjflt) && !isFinite(ncjdbl));

    cflt = std::complex<float> (fltInf, cflt.imag());
    cdbl = std::complex<double>(mdblInf, cdbl.imag());
    cjflt = conjugate<float>   (fltNaN, cjflt.imag());
    cjdbl = conjugate<double>  (dblInf, cjdbl.imag());

    // Both parts are non-finite.
    SimTK_TEST(!isFinite(cflt) && !isFinite(cdbl));
    SimTK_TEST(!isFinite(cjflt) && !isFinite(cjdbl));
    SimTK_TEST(!isFinite(ncflt) && !isFinite(ncdbl));
    SimTK_TEST(!isFinite(ncjflt) && !isFinite(ncjdbl));

    // Restore imaginary part to normal.
    cflt = std::complex<float>(cflt.real(), fltRegular);
    cdbl = std::complex<double>(cdbl.real(), dblRegular);
    cjflt = conjugate<float>(cjflt.real(), fltRegular);
    cjdbl = conjugate<double>(cjdbl.real(), dblRegular);

    // Real part only is non-finite;
    SimTK_TEST(!isFinite(cflt) && !isFinite(cdbl));
    SimTK_TEST(!isFinite(cjflt) && !isFinite(cjdbl));
    SimTK_TEST(!isFinite(ncflt) && !isFinite(ncdbl));
    SimTK_TEST(!isFinite(ncjflt) && !isFinite(ncjdbl));
}

void testSignBit() {
    const unsigned char ucm=0xff, ucz=0, ucp=27;
    const unsigned short usm=0xffff, usz=0, usp=2342;
    const unsigned int   uim=0xffffffff, uiz=0, uip=2342344;
    const unsigned long ulm=(unsigned long)-23423L, ulz=0, ulp=234234UL;
    const unsigned long long ullm=(unsigned long long)-234234234LL, ullz=0, ullp=234234234ULL;

    SimTK_TEST(!(signBit(ucm)||signBit(ucz)||signBit(ucp)));
    SimTK_TEST(!(signBit(usm)||signBit(usz)||signBit(usp)));
    SimTK_TEST(!(signBit(uim)||signBit(uiz)||signBit(uip)));
    SimTK_TEST(!(signBit(ulm)||signBit(ulz)||signBit(ulp)));
    SimTK_TEST(!(signBit(ullm)||signBit(ullz)||signBit(ullp)));
    
    // Note that signBit(char) doesn't exist.

    const signed char cm=-23, cz=0, cp=99;
    const short sm=-1234, sz=0, sp=23423;
    const int im=-2342343, iz=0, ip=29472383;
    const long lm=-43488, lz=0, lp=3454545;
    const long long llm=-2342342343433LL, llz=0, llp=874578478478574LL;

    SimTK_TEST(signBit(cm) && !(signBit(cz)||signBit(cp)));
    SimTK_TEST(signBit(sm) && !(signBit(sz)||signBit(sp)));
    SimTK_TEST(signBit(im) && !(signBit(iz)||signBit(ip)));
    SimTK_TEST(signBit(lm) && !(signBit(lz)||signBit(lp)));
    SimTK_TEST(signBit(llm) && !(signBit(llz)||signBit(llp)));

    const float fm=-12398.34f, fz=0, fp=4354.331f;
    const double dm=-234234.454, dz=0, dp=345345.2342;
    float mfz=-fz; double mdz=-dz;// -0

    SimTK_TEST(signBit(fm) && !(signBit(fz)||signBit(fp)));
    SimTK_TEST(signBit(dm) && !(signBit(dz)||signBit(dp)));
    SimTK_TEST(signBit(mfz) && signBit(mdz));

    // Note: signBit of negated float or double should be the
    // *same* as the underlying float or double; it is the
    // interpretation of that bit that is supposed to be 
    // different.
    const negator<float>& nfm=reinterpret_cast<const negator<float>&>(fm);
    const negator<float>& nfz=reinterpret_cast<const negator<float>&>(fz);
    const negator<float>& nfp=reinterpret_cast<const negator<float>&>(fp);
    const negator<float>& nmfz=reinterpret_cast<const negator<float>&>(mfz);
    const negator<double>& ndm=reinterpret_cast<const negator<double>&>(dm);
    const negator<double>& ndz=reinterpret_cast<const negator<double>&>(dz);
    const negator<double>& ndp=reinterpret_cast<const negator<double>&>(dp);
    const negator<double>& nmdz=reinterpret_cast<const negator<double>&>(mdz);

    SimTK_TEST(signBit(nfm) && !(signBit(nfz)||signBit(nfp)));
    SimTK_TEST(signBit(ndm) && !(signBit(ndz)||signBit(ndp)));
    SimTK_TEST(signBit(nmfz) && signBit(nmdz));

    const float fltInf = NTraits<float>::getInfinity();
    const double dblInf = NTraits<double>::getInfinity();
    const float mfltInf = -fltInf;
    const double mdblInf = -dblInf;

    SimTK_TEST(!signBit(fltInf) && !signBit(dblInf));
    SimTK_TEST(signBit(mfltInf) && signBit(mdblInf));
}


void testSign() {
    const unsigned char ucm=0xff, ucz=0, ucp=27;
    const unsigned short usm=0xffff, usz=0, usp=2342;
    const unsigned int   uim=0xffffffff, uiz=0, uip=2342344;
    const unsigned long ulm=(unsigned long)-23423L, ulz=0, ulp=234234UL;
    const unsigned long long ullm=(unsigned long long)-234234234LL, ullz=0, ullp=234234234ULL;

    SimTK_TEST(sign(ucm)==1 && sign(ucz)==0 && sign(ucp)==1);
    SimTK_TEST(sign(usm)==1 && sign(usz)==0 && sign(usp)==1);
    SimTK_TEST(sign(uim)==1 && sign(uiz)==0 && sign(uip)==1);
    SimTK_TEST(sign(ulm)==1 && sign(ulz)==0 && sign(ulp)==1);
    SimTK_TEST(sign(ullm)==1 && sign(ullz)==0 && sign(ullp)==1);

    // Note that sign(char) doesn't exist.

    const signed char cm=-23, cz=0, cp=99;
    const short sm=-1234, sz=0, sp=23423;
    const int im=-2342343, iz=0, ip=29472383;
    const long lm=-43488, lz=0, lp=3454545;
    const long long llm=-2342342343433LL, llz=0, llp=874578478478574LL;

    SimTK_TEST(sign(cm)==-1 && sign(cz)==0 && sign(cp)==1);
    SimTK_TEST(sign(sm)==-1 && sign(sz)==0 && sign(sp)==1);
    SimTK_TEST(sign(im)==-1 && sign(iz)==0 && sign(ip)==1);
    SimTK_TEST(sign(lm)==-1 && sign(lz)==0 && sign(lp)==1);
    SimTK_TEST(sign(llm)==-1 && sign(llz)==0 && sign(llp)==1);

    const float fm=-12398.34f, fz=0, fp=4354.331f;
    const double dm=-234234.454, dz=0, dp=345345.2342;
    float mfz=-fz; double mdz=-dz;// -0

    SimTK_TEST(sign(fm)==-1 && sign(fz)==0 && sign(fp)==1);
    SimTK_TEST(sign(dm)==-1 && sign(dz)==0 && sign(dp)==1);
    SimTK_TEST(sign(mfz)==0 && sign(mdz)==0); // doesn't matter if it's -0

    // Note: sign of negated float or double should be the
    // *opposite* as the underlying float or double.
    const negator<float>& nfm=reinterpret_cast<const negator<float>&>(fm);
    const negator<float>& nfz=reinterpret_cast<const negator<float>&>(fz);
    const negator<float>& nfp=reinterpret_cast<const negator<float>&>(fp);
    const negator<float>& nmfz=reinterpret_cast<const negator<float>&>(mfz);
    const negator<double>& ndm=reinterpret_cast<const negator<double>&>(dm);
    const negator<double>& ndz=reinterpret_cast<const negator<double>&>(dz);
    const negator<double>& ndp=reinterpret_cast<const negator<double>&>(dp);
    const negator<double>& nmdz=reinterpret_cast<const negator<double>&>(mdz);

    SimTK_TEST(sign(nfm)==1 && sign(nfz)==0 && sign(nfp)==-1);
    SimTK_TEST(sign(ndm)==1 && sign(ndz)==0 && sign(ndp)==-1);
    SimTK_TEST(sign(nmfz)==0 && sign(nmdz)==0); // doesn't matter if it's -0

    const float fltInf = NTraits<float>::getInfinity();
    const double dblInf = NTraits<double>::getInfinity();
    const float mfltInf = -fltInf;
    const double mdblInf = -dblInf;
    const negator<float>& nfltInf = reinterpret_cast<const negator<float>&>(fltInf);
    const negator<double>& ndblInf = reinterpret_cast<const negator<double>&>(dblInf);

    SimTK_TEST(sign(fltInf)==1 && sign(dblInf)==1);
    SimTK_TEST(sign(mfltInf)==-1 && sign(mdblInf)==-1);
    SimTK_TEST(sign(nfltInf)==-1 && sign(ndblInf)==-1);
}

void testSquareAndCube() {
    const float fval = -23.33f;
    const double dval = -234443.441;
    const negator<float>& nfval = reinterpret_cast<const negator<float>&>(fval);
    const negator<double>& ndval = reinterpret_cast<const negator<double>&>(dval);

    // Basic test.
    SimTK_TEST_NUMEQ(square(fval), fval*fval);
    SimTK_TEST_NUMEQ(square(dval), dval*dval);
    SimTK_TEST_NUMEQ(cube(fval), fval*fval*fval);
    SimTK_TEST_NUMEQ(cube(dval), dval*dval*dval);

    // Test scalar negators.
    SimTK_TEST_NUMEQ(square(nfval), nfval*nfval);
    SimTK_TEST_NUMEQ(square(nfval), fval*fval);
    SimTK_TEST_NUMEQ(square(ndval), ndval*ndval);
    SimTK_TEST_NUMEQ(square(ndval), dval*dval);
    SimTK_TEST_NUMEQ(cube(nfval), nfval*nfval*nfval);
    SimTK_TEST_NUMEQ(cube(nfval), -fval*fval*fval);
    SimTK_TEST_NUMEQ(cube(ndval), ndval*ndval*ndval);
    SimTK_TEST_NUMEQ(cube(ndval), -dval*dval*dval);

    // Create complex and conjugate values.

    std::complex<float> fc(-234.343f, 45345e7f);
    std::complex<double> dc(-234.343, 45345e7);
    conjugate<float> fcj(-19.1e3f, -454.234f);
    conjugate<double> dcj(-19.1e3, -454.234);

    // Manual conjugates
    std::complex<float>  fcmj(fcj.real(), fcj.imag());
    std::complex<double> dcmj(dcj.real(), dcj.imag());
    SimTK_TEST(fcj == fcmj);    // sign change only; should be exact
    SimTK_TEST(dcj == dcmj);
    SimTK_TEST_NUMEQ(fcj*fcj, fcmj*fcmj);
    SimTK_TEST_NUMEQ(dcj*dcj, dcmj*dcmj);
    SimTK_TEST_NUMEQ(fcj*fcj*fcj, fcmj*fcmj*fcmj);
    SimTK_TEST_NUMEQ(dcj*dcj*dcj, dcmj*dcmj*dcmj);

    // Negators of complex an conjugate.
    negator<std::complex<float> >&   nfc  = reinterpret_cast<negator<std::complex<float> >&> (fc);
    negator<std::complex<double> >&  ndc  = reinterpret_cast<negator<std::complex<double> >&>(dc);
    negator<conjugate<float> >&      nfcj = reinterpret_cast<negator<conjugate<float> >&>    (fcj);
    negator<conjugate<double> >&     ndcj = reinterpret_cast<negator<conjugate<double> >&>   (dcj);

    // Change of sign should be exact.
    SimTK_TEST(nfc == -fc);
    SimTK_TEST(ndc == -dc);
    SimTK_TEST(nfcj == -fcj);
    SimTK_TEST(ndcj == -dcj);


    // Basic complex and conjugate tests.
    SimTK_TEST_NUMEQ(square(fc), fc*fc);
    SimTK_TEST_NUMEQ(cube(fc), fc*fc*fc);
    SimTK_TEST_NUMEQ(square(dc), dc*dc);
    SimTK_TEST_NUMEQ(cube(dc), dc*dc*dc);
    SimTK_TEST_NUMEQ(square(fcj), fcj*fcj);
    SimTK_TEST_NUMEQ(cube(fcj), fcj*fcj*fcj);
    SimTK_TEST_NUMEQ(square(dcj), dcj*dcj);
    SimTK_TEST_NUMEQ(cube(dcj), dcj*dcj*dcj);

    // Tests involving negators of complex and conjugate.
    SimTK_TEST_NUMEQ(square(nfc), nfc*nfc); 
    SimTK_TEST_NUMEQ(square(nfc), fc*fc);
    SimTK_TEST_NUMEQ(square(ndc), ndc*ndc);
    SimTK_TEST_NUMEQ(square(ndc), dc*dc);

    SimTK_TEST_NUMEQ(cube(nfc), nfc*nfc*nfc); 
    SimTK_TEST_NUMEQ(cube(nfc), -fc*fc*fc);
    SimTK_TEST_NUMEQ(cube(ndc), ndc*ndc*ndc);
    SimTK_TEST_NUMEQ(cube(ndc), -dc*dc*dc);

    SimTK_TEST_NUMEQ(square(nfcj), nfcj*nfcj); 
    SimTK_TEST_NUMEQ(square(nfcj), fcj*fcj);
    SimTK_TEST_NUMEQ(square(ndcj), ndcj*ndcj);
    SimTK_TEST_NUMEQ(square(ndcj), dcj*dcj);

    SimTK_TEST_NUMEQ(cube(nfcj), nfcj*nfcj*nfcj); 
    SimTK_TEST_NUMEQ(cube(nfcj), -fcj*fcj*fcj);
    SimTK_TEST_NUMEQ(cube(ndcj), ndcj*ndcj*ndcj);
    SimTK_TEST_NUMEQ(cube(ndcj), -dcj*dcj*dcj);
}

int main() {
    SimTK_START_TEST("TestScalar");

        SimTK_SUBTEST(testIsNaN);
        SimTK_SUBTEST(testIsInf);
        SimTK_SUBTEST(testIsFinite);
        SimTK_SUBTEST(testSignBit);
        SimTK_SUBTEST(testSign);
        SimTK_SUBTEST(testSquareAndCube);

    SimTK_END_TEST();
}
