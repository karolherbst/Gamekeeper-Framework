/*
 * GameKeeper Framework
 *
 * Copyright (C) 2013 Karol Herbst <gamekeeper@karolherbst.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef GAMEKEEPER_MODEL_MACROS_H
#define GAMEKEEPER_MODEL_MACROS_H 1

#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/punctuation/paren.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>

// first some properties
#define GK_MODEL_OPTIONAL 0
#define GK_MODEL_REQUIRED 1

/**
 * this macro generates const & getter() const method definitions
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] pre all the definitions which shall come before the actual getter like visibility
 * @param[in] type the Type of the property the getter is for
 * @param[in] name the name of the property begining with an Uppercase letter
 * @param[in] post all defnitions which shall come after the getter like "override" or "= 0"
 */
#define GK_GETTER_DEF(pre, type, name, post) \
pre const type & BOOST_PP_CAT(get,name)() const post

/**
 * this macro generates const & getter() const method implementation
 *
 * the implementation will access the expression of \a provider which will be directly returned
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] clazz the class for which the getter is for
 * @param[in] type the Type of the property the getter is for
 * @param[in] name the name of the property begining with an Uppercase letter
 * @param[in] post all defnitions which shall come after the getter like "override" or "= 0"
 * @param[in] provider the property which shall be returned
 */
#define GK_GETTER_IMPL(clazz, type, name, post, provider) \
const type & BOOST_PP_CAT(clazz::get, name)() const post { return provider; }


/**
 * this macro generates const & getter() const method implementation
 *
 * the implementation will access the expression of @a provider which will be called directly and its result returned
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] clazz the class for which the getter is for
 * @param[in] type the Type of the property the getter is for
 * @param[in] name the name of the property begining with an Uppercase letter
 * @param[in] post all defnitions which shall come after the getter like "override" or "= 0"
 * @param[in] provider the method which result will be directly returned
 */
#define GK_GETTER_IMPL_WRAPPER(clazz, type, name, post, provider) \
const type & BOOST_PP_CAT(clazz::get, name)() const post { return provider(); }

/**
 * this macro generates void setter(const &) and void setter(&&) method definitions
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] pre all the definitions which shall come before the actual getter like visibility
 * @param[in] type the Type of the property the getter is for
 * @param[in] name the name of the property begining with an Uppercase letter
 * @param[in] post all defnitions which shall come after the getter like "override" or "= 0"
 */
#define GK_SETTER_DEF(pre, type, name, post) \
pre void BOOST_PP_CAT(set, name)(const type &) post; \
pre void BOOST_PP_CAT(set, name)(type &&) post \

/**
 * this macro generates void setter(const &) and void setter(&&) method implementations
 *
 * the implementation will access the expression of @a provider which will be directly returned
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] clazz the class for which the getter is for
 * @param[in] type the Type of the property the getter is for
 * @param[in] name the name of the property begining with an Uppercase letter
 * @param[in] post all defnitions which shall come after the getter like "override" or "= 0"
 * @param[in] destination the property which shall be returned
 */
#define GK_SETTER_IMPL(clazz, type, name, post, destination) \
void BOOST_PP_CAT(clazz::set, name)(const type & value) post { destination = value; } \
void BOOST_PP_CAT(clazz::set, name)(type && value) post { destination = value; } \

/**
 * this macro generates void setter(const &) and void setter(&&) method implementations
 *
 * the implementation will access the expression of @a provider which will be called directly and its result returned
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] clazz the class for which the getter is for
 * @param[in] type the Type of the property the getter is for
 * @param[in] name the name of the property begining with an Uppercase letter
 * @param[in] post all defnitions which shall come after the getter like "override" or "= 0"
 * @param[in] destination the setter which will be called
 */
#define GK_SETTER_IMPL_WRAPPER(clazz, type, name, post, destination) \
void BOOST_PP_CAT(clazz::set, name)(const type & value) post { destination(value); } \
void BOOST_PP_CAT(clazz::set, name)(type && value) post { destination(value); } \

/**
 * this macro is used to map a game model delceration entry to the Interface Building macros
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] r BOOST_PP_FOR repetition
 * @param[in] data data elemns in form of (pre, post)
 * @param[in] elem the data element
 * @param[in] impl the macro to expand
 */
#define GK_BUILD_INTERFACE_IMPL(r, data, elem, impl) \
impl( \
	BOOST_PP_TUPLE_ELEM(2, 0, data), \
	BOOST_PP_TUPLE_ELEM(4, 0, elem), \
	BOOST_PP_TUPLE_ELEM(4, 2, elem), \
	BOOST_PP_TUPLE_ELEM(2, 1, data));

/**
 * this macro is used to map a game model delceration entry to the Implementation Building macros
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] r BOOST_PP_FOR repetition
 * @param[in] data data elemns in form of (clazz, pre, post)
 * @param[in] elem the data element
 * @param[in] impl the macro to expand
 */
#define GK_BUILD_IMPLEMENTATION_IMPL(r, data, elem, impl) \
impl( \
	BOOST_PP_TUPLE_ELEM(3, 0, data), \
	BOOST_PP_TUPLE_ELEM(4, 0, elem), \
	BOOST_PP_TUPLE_ELEM(4, 2, elem), \
	BOOST_PP_TUPLE_ELEM(3, 1, data), \
	BOOST_PP_TUPLE_ELEM(3, 2, data)->BOOST_PP_TUPLE_ELEM(4, 1, elem));

/**
 * this macro is used to map a game model delceration entry to the Implementation Wrapper Building macros
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] r BOOST_PP_FOR repetition
 * @param[in] data data elemnt in form of (clazz, pre, post)
 * @param[in] elem the data element
 * @param[in] impl the macro to expand
 */
#define GK_BUILD_IMPLEMENTATION_WRAPPER_IMPL(r, data, elem, impl, method) \
impl( \
	BOOST_PP_TUPLE_ELEM(3, 0, data), \
	BOOST_PP_TUPLE_ELEM(4, 0, elem), \
	BOOST_PP_TUPLE_ELEM(4, 2, elem), \
	BOOST_PP_TUPLE_ELEM(3, 1, data), \
	BOOST_PP_TUPLE_ELEM(3, 2, data)->BOOST_PP_CAT(method,BOOST_PP_TUPLE_ELEM(4, 2, elem)));

/**
 * this macro is used to just call the right macro passed in GK_MODEL_DO_FORALL
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] impl the method which shall be called
 * @param[in] elem the data element
 */
#define GK_MODEL_CALL_IMPL(impl, elem) \
impl( \
	BOOST_PP_TUPLE_ELEM(4, 0, elem), \
	BOOST_PP_TUPLE_ELEM(4, 1, elem), \
	BOOST_PP_TUPLE_ELEM(4, 2, elem))

/**
 * implementation for the GK_MODEL_DO_FORALL macro
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] r BOOST_PP_FOR repetition
 * @param[in] data data element in form of (required, optional)
 * @param[in] elem the data element
 */
#define GK_MODEL_DO_FORALL_IMPL(r, data, elem) \
GK_MODEL_CALL_IMPL(BOOST_PP_IIF(BOOST_PP_TUPLE_ELEM(4, 3, elem), \
                                BOOST_PP_TUPLE_ELEM(2, 0, data), \
                                BOOST_PP_TUPLE_ELEM(2, 1, data)), \
                   elem)

// some wrapper function which only save some locs
#define GK_BUILD_GET_INTERFACE_IMPL(r, data, elem) GK_BUILD_INTERFACE_IMPL(r, data, elem, GK_GETTER_DEF)
#define GK_BUILD_SET_INTERFACE_IMPL(r, data, elem) GK_BUILD_INTERFACE_IMPL(r, data, elem, GK_SETTER_DEF)
#define GK_BUILD_GET_IMPLEMENTATION_IMPL(r, data, elem) GK_BUILD_IMPLEMENTATION_IMPL(r, data, elem, GK_GETTER_IMPL)
#define GK_BUILD_SET_IMPLEMENTATION_IMPL(r, data, elem) GK_BUILD_IMPLEMENTATION_IMPL(r, data, elem, GK_SETTER_IMPL)
#define GK_BUILD_GET_IMPLEMENTATION_WRAPPER_IMPL(r, data, elem) GK_BUILD_IMPLEMENTATION_WRAPPER_IMPL(r, data, elem, GK_GETTER_IMPL_WRAPPER, get)
#define GK_BUILD_SET_IMPLEMENTATION_WRAPPER_IMPL(r, data, elem) GK_BUILD_IMPLEMENTATION_WRAPPER_IMPL(r, data, elem, GK_SETTER_IMPL_WRAPPER, set)
#define GK_BUILD_MEMBER_IMPL(r, data, elem) BOOST_PP_TUPLE_ELEM(3, 0, elem) BOOST_PP_TUPLE_ELEM(3, 1, elem);

/**
 * this macros generates a getter interface out of the given @a model
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the given model
 * @param[in] pre this will be appended before the generated method
 * @param[in] post this will be appended after the generated method
 */
#define GK_BUILD_GET_INTERFACE(model, pre, post) BOOST_PP_LIST_FOR_EACH(GK_BUILD_GET_INTERFACE_IMPL, (pre, post), BOOST_PP_TUPLE_TO_LIST(model))

/**
 * this macros generates a setter interface out of the given @a model
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the given model
 * @param[in] pre this will be appended before the generated method
 * @param[in] post this will be appended after the generated method
 */
#define GK_BUILD_SET_INTERFACE(model, pre, post) BOOST_PP_LIST_FOR_EACH(GK_BUILD_SET_INTERFACE_IMPL, (pre, post), BOOST_PP_TUPLE_TO_LIST(model))

/**
 * this just calls GK_BUILD_GET_INTERFACE and GK_BUILD_SET_INTERFACE
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the given model
 * @param[in] pre this will be appended before the generated method
 * @param[in] post this will be appended after the generated method
 */
#define GK_BUILD_INTERFACE(model, pre, post) GK_BUILD_GET_INTERFACE(model, pre, post) GK_BUILD_SET_INTERFACE(model, pre, post)

/**
 * this macros generates a getter implementations out of the given @a model. Members of @a ptr will be returned
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the given model
 * @param[in] clazz the class to which the getters belong
 * @param[in] post this will be appended at the end of the generated method decleration
 * @param[in] ptr the pointer to the object where the members are stored, usually this or this->pimpl
 */
#define GK_BUILD_GET_IMPLEMENTATION(model, clazz, post, ptr) BOOST_PP_LIST_FOR_EACH(GK_BUILD_GET_IMPLEMENTATION_IMPL, (clazz, post, ptr), BOOST_PP_TUPLE_TO_LIST(model))

/**
 * this macros generates a setter implementations out of the given @a model. Members of @a ptr will be set
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the given model
 * @param[in] clazz the class to which the getters belong
 * @param[in] post this will be appended at the end of the generated method decleration
 * @param[in] ptr the pointer to the object where the members are stored, usually this or this->pimpl
 */
#define GK_BUILD_SET_IMPLEMENTATION(model, clazz, post, ptr) BOOST_PP_LIST_FOR_EACH(GK_BUILD_SET_IMPLEMENTATION_IMPL, (clazz, post, ptr), BOOST_PP_TUPLE_TO_LIST(model))

/**
 * this just calls GK_BUILD_GET_IMPLEMENTATION and GK_BUILD_SET_IMPLEMENTATION
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the given model
 * @param[in] clazz the class to which the getters belong
 * @param[in] post this will be appended at the end of the generated method decleration
 * @param[in] ptr the pointer to the object where the members are stored, usually this or this->pimpl
 */
#define GK_BUILD_IMPLEMENTATION(model, clazz, post, ptr) GK_BUILD_GET_IMPLEMENTATION(model, clazz, post, ptr) GK_BUILD_SET_IMPLEMENTATION(model, clazz, post, ptr)

/**
 * this macros generates a getter implementations out of the given @a model. Getters of @a ptr will be used
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the given model
 * @param[in] clazz the class to which the getters belong
 * @param[in] post this will be appended at the end of the generated method decleration
 * @param[in] ptr the pointer to the object with the getters. Usually this or this->pimpl
 */
#define GK_BUILD_GET_IMPLEMENTATION_WRAPPER(model, clazz, post, ptr) BOOST_PP_LIST_FOR_EACH(GK_BUILD_GET_IMPLEMENTATION_WRAPPER_IMPL, (clazz, post, ptr), BOOST_PP_TUPLE_TO_LIST(model))

/**
 * this macros generates a setter implementations out of the given @a model. Setters of @a ptr will be used
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the given model
 * @param[in] clazz the class to which the getters belong
 * @param[in] post this will be appended at the end of the generated method decleration
 * @param[in] ptr the pointer to the object with the getters. Usually this or this->pimpl
 */
#define GK_BUILD_SET_IMPLEMENTATION_WRAPPER(model, clazz, post, ptr) BOOST_PP_LIST_FOR_EACH(GK_BUILD_SET_IMPLEMENTATION_WRAPPER_IMPL, (clazz, post, ptr), BOOST_PP_TUPLE_TO_LIST(model))


/**
 * this just calls GK_BUILD_GET_IMPLEMENTATION_WRAPPER and GK_BUILD_SET_IMPLEMENTATION_WRAPPER
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the given model
 * @param[in] clazz the class to which the getters belong
 * @param[in] post this will be appended at the end of the generated method decleration
 * @param[in] ptr the pointer to the object where the members are stored, usually this or this->pimpl
 */
#define GK_BUILD_IMPLEMENTATION_WRAPPER(model, clazz, post, ptr) GK_BUILD_GET_IMPLEMENTATION_WRAPPER(model, clazz, post, ptr) GK_BUILD_SET_IMPLEMENTATION_WRAPPER(model, clazz, post, ptr)

/**
 * this macro just builds the member of the model in the following way: type name;
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the model
 */
#define GK_BUILD_MEMBER(model) BOOST_PP_LIST_FOR_EACH(GK_BUILD_MEMBER_IMPL,, BOOST_PP_TUPLE_TO_LIST(model))

/**
 * this macro executes the @a required or the @a optional macro given depending on the required parameter of the model. the interface of the macros is: (type, name, Name)
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param[in] model the model
 * @param[in] required the macro which should be applied to required members
 * @param[in] optional the macro which should be applied to optional members
 */
#define GK_MODEL_DO_FORALL(model, required, optional) BOOST_PP_LIST_FOR_EACH(GK_MODEL_DO_FORALL_IMPL, (required, optional), BOOST_PP_TUPLE_TO_LIST(model))

#endif //GAMEKEEPER_MODEL_MACROS_H
