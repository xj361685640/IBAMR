// ---------------------------------------------------------------------
//
// Copyright (c) 2006 - 2019 by the IBAMR developers
// All rights reserved.
//
// This file is part of IBAMR.
//
// IBAMR is free software and is distributed under the 3-clause BSD
// license. The full text of the license can be found in the file
// COPYRIGHT at the top level directory of IBAMR.
//
// ---------------------------------------------------------------------

/////////////////////// INCLUDE GUARD ////////////////////////////////////

#ifndef included_IBAMR_IBLagrangianForceStrategy
#define included_IBAMR_IBLagrangianForceStrategy

/////////////////////////////// INCLUDES /////////////////////////////////////

#include <ibamr/config.h>

#include "tbox/DescribedClass.h"
#include "tbox/Pointer.h"

#include "petscmat.h"

#include <vector>

namespace IBTK
{
class LData;
class LDataManager;
} // namespace IBTK
namespace SAMRAI
{
namespace hier
{
template <int DIM>
class PatchHierarchy;
} // namespace hier
} // namespace SAMRAI

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBAMR
{
/*!
 * \brief Class IBLagrangianForceStrategy provides a generic interface for
 * specifying nodal forces (\em not force densities) on the Lagrangian
 * curvilinear mesh.
 *
 * \note Implementations of class IBLagrangianForceStrategy must compute the
 * total nodal forces.  In particular, they \em must \em not compute the nodal
 * force densities.
 *
 * \see IBBeamForceGen
 * \see IBLagrangianForceStrategySet
 * \see IBSpringForceGen
 * \see IBStandardForceGen
 * \see IBTargetPointForceGen
 */
class IBLagrangianForceStrategy : public virtual SAMRAI::tbox::DescribedClass
{
public:
    /*!
     * \brief Default constructor.
     */
    IBLagrangianForceStrategy() = default;

    /*!
     * \brief Virtual destructor.
     */
    virtual ~IBLagrangianForceStrategy() = default;

    /*!
     * \brief Set the current and new times for the present timestep.
     *
     * \note A default empty implementation is provided.
     */
    virtual void setTimeInterval(double current_time, double new_time);

    /*!
     * \brief Setup the data needed to compute the curvilinear force on the
     * specified level of the patch hierarchy.
     *
     * \note A default empty implementation is provided.
     */
    virtual void initializeLevelData(SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
                                     int level_number,
                                     double init_data_time,
                                     bool initial_time,
                                     IBTK::LDataManager* l_data_manager);

    /*!
     * \brief Compute the curvilinear force generated by the given configuration
     * of the curvilinear mesh.
     *
     * \note Nodal forces computed by implementations of this method must be \em
     * added to the force vector.
     *
     * \note A default implementation is provided that results in an assertion
     * failure.
     */
    virtual void computeLagrangianForce(SAMRAI::tbox::Pointer<IBTK::LData> F_data,
                                        SAMRAI::tbox::Pointer<IBTK::LData> X_data,
                                        SAMRAI::tbox::Pointer<IBTK::LData> U_data,
                                        SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
                                        int level_number,
                                        double data_time,
                                        IBTK::LDataManager* l_data_manager);

    /*!
     * \brief Compute the non-zero structure of the force Jacobian matrix.
     *
     * \note Elements indices must be global PETSc indices.
     *
     * \note A default implementation is provided that results in an assertion
     * failure.
     */
    virtual void
    computeLagrangianForceJacobianNonzeroStructure(std::vector<int>& d_nnz,
                                                   std::vector<int>& o_nnz,
                                                   SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
                                                   int level_number,
                                                   IBTK::LDataManager* l_data_manager);

    /*!
     * \brief Compute the Jacobian of the force with respect to the present
     * structure configuration and velocity.
     *
     * \note The elements of the Jacobian should be "accumulated" in the
     * provided matrix J.
     *
     * \note A default implementation is provided that results in an assertion
     * failure.
     */
    virtual void computeLagrangianForceJacobian(Mat& J_mat,
                                                MatAssemblyType assembly_type,
                                                double X_coef,
                                                SAMRAI::tbox::Pointer<IBTK::LData> X_data,
                                                double U_coef,
                                                SAMRAI::tbox::Pointer<IBTK::LData> U_data,
                                                SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
                                                int level_number,
                                                double data_time,
                                                IBTK::LDataManager* l_data_manager);

    /*!
     * \brief Compute the potential energy with respect to the present structure
     * configuration and velocity.
     *
     * \note A default implementation is provided that results in an assertion
     * failure.
     */
    virtual double computeLagrangianEnergy(SAMRAI::tbox::Pointer<IBTK::LData> X_data,
                                           SAMRAI::tbox::Pointer<IBTK::LData> U_data,
                                           SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
                                           int level_number,
                                           double data_time,
                                           IBTK::LDataManager* l_data_manager);

private:
    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    IBLagrangianForceStrategy(const IBLagrangianForceStrategy& from) = delete;

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    IBLagrangianForceStrategy& operator=(const IBLagrangianForceStrategy& that) = delete;
};
} // namespace IBAMR

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_IBAMR_IBLagrangianForceStrategy
