#include <iostream>

#include "../include/Record.hpp"


Record
Record::makeScalarRecord(Dimension rd)
{
    return Record(rd, {}, true);
}

Record
Record::makeTensorRecord(Dimension rd,
                         std::initializer_list< std::string > il)
{
    return Record(rd, il, il.size() == 0);
}

Record::Record(Dimension dim,
               std::initializer_list< std::string > comps,
               bool isRecordComponent)
        : m_components{std::map< std::string, RecordComponent >()},
          m_isComponent{isRecordComponent},
          m_dimension{dim}
{
    for( std::string const& component : comps )
    {
        using it_t = std::map< std::string, RecordComponent >::iterator;
        std::pair< it_t, bool > res =
                m_components.emplace(component,
                                     RecordComponent());
        res.first->second.setAttribute("unitSI", static_cast<double>(1));
    }
    setAttribute("unitDimension",
                 std::array< double, 7 >{{0., 0., 0., 0., 0., 0., 0.}});
    setTimeOffset(0);
}

Record::Record(Record const& r)
        : Attributable(r),
          scalar{r.scalar},
          m_components{r.m_components},
          m_isComponent{r.m_isComponent},
          m_dimension{r.m_dimension}
{ }

std::array< double, 7 >
Record::unitDimension() const
{
    return boost::get< std::array< double, 7 > >(getAttribute("unitDimension").getResource());
}

Record&
Record::setUnitDimension(std::map< Record::UnitDimension, double > const& udim)
{
    if( udim.size() != 0 )
    {
        std::array< double, 7 > unitDimension = this->unitDimension();
        for( auto const& entry : udim )
        {
            unitDimension[static_cast<size_t>(entry.first)] = entry.second;
        }
        setAttribute("unitDimension", unitDimension);
    }
    return *this;
}

float
Record::timeOffset() const
{
    return boost::get< float >(getAttribute("timeOffset").getResource());
}

Record&
Record::setTimeOffset(float timeOffset)
{
    setAttribute("timeOffset", timeOffset);
    return *this;
}

RecordComponent&
Record::operator[](std::string const& component)
{
    auto it = m_components.find(component);
    if( it != m_components.end() )
    {
        return it->second;
    }
    throw std::runtime_error("Unknown recordComponent " + component);
}
