// ---------------------------------------------------------------------
//
// Copyright (c) 2010 - 2019 by the IBAMR developers
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

#ifndef included_IBAMR_IBRodForceSpec
#define included_IBAMR_IBRodForceSpec

/////////////////////////////// INCLUDES /////////////////////////////////////

#include <ibamr/config.h>

#include "ibtk/Streamable.h"
#include "ibtk/StreamableFactory.h"
#include "ibtk/ibtk_utilities.h"

#include "tbox/Pointer.h"

#include <array>
#include <vector>

namespace SAMRAI
{
namespace hier
{
template <int DIM>
class IntVector;
} // namespace hier
namespace tbox
{
class AbstractStream;
} // namespace tbox
} // namespace SAMRAI

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBAMR
{
/*!
 * \brief Class IBRodForceSpec encapsulates the data necessary to compute the
 * forces generated by a network of Kirchhoff rods at a single node of the
 * Lagrangian mesh.
 */
class IBRodForceSpec : public IBTK::Streamable
{
public:
    static const int NUM_MATERIAL_PARAMS = 10;

    /*!
     * \brief Register this class and its factory class with the singleton
     * IBTK::StreamableManager object.  This method must be called before any
     * IBRodForceSpec objects are created.
     *
     * \note This method is collective on all MPI processes.  This is done to
     * ensure that all processes employ the same class ID for the
     * IBRodForceSpec class.
     */
    static void registerWithStreamableManager();

    /*!
     * \brief Returns a boolean indicating whether the class has been registered
     * with the singleton IBTK::StreamableManager object.
     */
    static bool getIsRegisteredWithStreamableManager();

    /*!
     * The unique class ID for this object type assigned by the
     * IBTK::StreamableManager.
     */
    static int STREAMABLE_CLASS_ID;

    /*!
     * \brief Default constructor.
     */
    IBRodForceSpec(unsigned int num_rods = 0);

    /*!
     * \brief Alternate constructor.
     */
    IBRodForceSpec(int master_idx,
                   const std::vector<int>& next_idxs,
                   const std::vector<std::array<double, NUM_MATERIAL_PARAMS> >& material_params);

    /*!
     * \brief Destructor.
     */
    ~IBRodForceSpec();

    /*!
     * \return The number of rods attached to the master node.
     */
    unsigned int getNumberOfRods() const;

    /*!
     * \return A const reference to the master node index.
     */
    const int& getMasterNodeIndex() const;

    /*!
     * \return A non-const reference to the master node index.
     */
    int& getMasterNodeIndex();

    /*!
     * \return A const reference to the next node indices for the rods attached
     * to the master node.
     */
    const std::vector<int>& getNextNodeIndices() const;

    /*!
     * \return A non-const reference to the next node indices for the rods
     * attached to the master node.
     */
    std::vector<int>& getNextNodeIndices();

    /*!
     * \return A const reference to the material parameters of the rods attached
     * to the master node.
     */
    const std::vector<std::array<double, NUM_MATERIAL_PARAMS> >& getMaterialParams() const;

    /*!
     * \return A non-const reference to the material parameters of the rods
     * attached to the master node.
     */
    std::vector<std::array<double, NUM_MATERIAL_PARAMS> >& getMaterialParams();

    /*!
     * \brief Return the unique identifier used to specify the
     * IBTK::StreamableFactory object used by the IBTK::StreamableManager to
     * extract Streamable objects from data streams.
     */
    int getStreamableClassID() const override;

    /*!
     * \brief Return an upper bound on the amount of space required to pack the
     * object to a buffer.
     */
    size_t getDataStreamSize() const override;

    /*!
     * \brief Pack data into the output stream.
     */
    void packStream(SAMRAI::tbox::AbstractStream& stream) override;

private:
    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    IBRodForceSpec(const IBRodForceSpec& from) = delete;

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    IBRodForceSpec& operator=(const IBRodForceSpec& that) = delete;

    /*!
     * Data required to define the spring forces.
     */
    int d_master_idx = IBTK::invalid_index;
    std::vector<int> d_next_idxs;
    std::vector<std::array<double, NUM_MATERIAL_PARAMS> > d_material_params;

    /*!
     * \brief A factory class to rebuild IBRodForceSpec objects from
     * SAMRAI::tbox::AbstractStream data streams.
     */
    class Factory : public IBTK::StreamableFactory
    {
    public:
        /*!
         * \brief Destructor.
         */
        ~Factory() = default;

        /*!
         * \brief Return the unique identifier used to specify the
         * IBTK::StreamableFactory object used by the IBTK::StreamableManager to
         * extract IBRodForceSpec objects from data streams.
         */
        int getStreamableClassID() const override;

        /*!
         * \brief Set the unique identifier used to specify the
         * IBTK::StreamableFactory object used by the IBTK::StreamableManager to
         * extract IBRodForceSpec objects from data streams.
         */
        void setStreamableClassID(int class_id) override;

        /*!
         * \brief Build an IBRodForceSpec object by unpacking data from the data
         * stream.
         */
        SAMRAI::tbox::Pointer<IBTK::Streamable> unpackStream(SAMRAI::tbox::AbstractStream& stream,
                                                             const SAMRAI::hier::IntVector<NDIM>& offset) override;

    private:
        /*!
         * \brief Default constructor.
         */
        Factory();

        /*!
         * \brief Copy constructor.
         *
         * \note This constructor is not implemented and should not be used.
         *
         * \param from The value to copy to this object.
         */
        Factory(const Factory& from) = delete;

        /*!
         * \brief Assignment operator.
         *
         * \note This operator is not implemented and should not be used.
         *
         * \param that The value to assign to this object.
         *
         * \return A reference to this object.
         */
        Factory& operator=(const Factory& that) = delete;

        friend class IBRodForceSpec;
    };
    using IBRodForceSpecFactory = IBRodForceSpec::Factory;
};
} // namespace IBAMR

/////////////////////////////// INLINE ///////////////////////////////////////

#include "ibamr/private/IBRodForceSpec-inl.h" // IWYU pragma: keep

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_IBAMR_IBRodForceSpec
