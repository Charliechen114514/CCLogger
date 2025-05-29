#pragma once

/**
 * @brief   DISABLE_COPY is such a macro helps disable
 *          The default copy behaviors
 *
 */
#define DISABLE_COPY(ClassType)           \
	ClassType(const ClassType&) = delete; \
	const ClassType& operator=(const ClassType&) = delete;

#define DISABLE_COPY_MOVE(ClassType)                       \
	ClassType(const ClassType&) = delete;                  \
	const ClassType& operator=(const ClassType&) = delete; \
	ClassType(ClassType&&) = delete;                       \
	const ClassType& operator=(ClassType&&) = delete;

/**
 * @brief For Developers who wanna get a convienent
 *        Getter and Setter, this is the macro that can low level
 *        specify the get set types, but, if you dont means to specify
 *        the type, you should use the macro PROPERTY_GET_SET instead of
 *        this one, as types below is manually while PROPERTY_GET_SET's type
 *        is automatically detected by the compiler
 * @param type the type of the property
 * @param property the property name
 */
#define __PROPERTY_GET_SET(type, property)           \
	type get_##property() const {                    \
		return property;                             \
	}                                                \
	inline void set_##property(const type& _value) { \
		this->property = _value;                     \
	}

/**
 * @brief Macro fast generate the getter and setter,
 * @param property the property name
 *
 */
#define PROPERTY_GET_SET(property) \
	__PROPERTY_GET_SET(typeof(property), property)
