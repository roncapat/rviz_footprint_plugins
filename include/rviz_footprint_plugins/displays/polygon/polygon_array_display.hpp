#ifndef RVIZ_FOOTPRINT_PLUGINS__DISPLAYS__POLYGON__POLYGON_ARRAY_DISPLAY_HPP_
#define RVIZ_FOOTPRINT_PLUGINS__DISPLAYS__POLYGON__POLYGON_ARRAY_DISPLAY_HPP_

#include "trav_msgs/msg/polygon_array.hpp"
#include "rviz_common/message_filter_display.hpp"
#include "rviz_footprint_plugins/visibility_control.hpp"

namespace Ogre
{
class ManualObject;
}

namespace rviz_common
{
namespace properties
{
class ColorProperty;
class FloatProperty;
}  // namespace properties
}  // namespace rviz_common

namespace rviz_footprint_plugins
{
namespace displays
{

/**
 * \class PolygonArrayDisplay
 * \brief Displays an trav_msgs::PolygonArray message
 */
class RVIZ_FOOTPRINT_PLUGINS_PUBLIC PolygonArrayDisplay : public
  rviz_common::MessageFilterDisplay<trav_msgs::msg::PolygonArray>
{
  Q_OBJECT

public:
  PolygonArrayDisplay();
  ~PolygonArrayDisplay() override;

  void onInitialize() override;

  void reset() override;

protected:
  void processMessage(trav_msgs::msg::PolygonArray::ConstSharedPtr msg) override;

  Ogre::ManualObject *manual_object_;
  Ogre::MaterialPtr material_;

  rviz_common::properties::ColorProperty * color_property_;
  rviz_common::properties::FloatProperty * alpha_property_;
};

}  // namespace displays
}  // namespace rviz_footprint_plugins

#endif  // RVIZ_FOOTPRINT_PLUGINS__DISPLAYS__POLYGON__POLYGON_ARRAY_DISPLAY_HPP_
