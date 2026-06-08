/*
 *	MetaCall Library by Parra Studios
 *	A library for providing a foreign function interface calls.
 *
 *	Copyright (C) 2016 - 2026 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 */

#include <gtest/gtest.h>

#include <metacall/metacall.h>
#include <metacall/metacall_loaders.h>
#include <metacall/metacall_value.h>

class metacall_python_conversion_test : public testing::Test
{
public:
};

TEST_F(metacall_python_conversion_test, DefaultConstructor)
{
	metacall_print_info();

	metacall_log_null();

	ASSERT_EQ((int)0, (int)metacall_initialize());

	{
		static const char script[] =
			"def identity(x):\n"
			"    return x\n";

		void *handle = NULL;
		void *ret;

		ASSERT_EQ((int)0, (int)metacall_load_from_memory("py", script, sizeof(script), &handle));

		{
			void *args[1] = { metacall_value_create(METACALL_INT) };

			ret = metacallhv(handle, "identity", args);

			EXPECT_NE((void *)NULL, (void *)ret);

			if (ret != NULL)
			{
				std::cout
					<< "INT => "
					<< metacall_value_id_name(metacall_value_id(ret))
					<< std::endl;

				metacall_value_destroy(ret);
			}

			metacall_value_destroy(args[0]);
		}

		for (size_t id = 0; id < METACALL_SIZE; ++id)
		{
			if((enum metacall_value_id)id == METACALL_CLASS || (enum metacall_value_id)id == METACALL_OBJECT || (enum metacall_value_id)id == METACALL_EXCEPTION || (enum metacall_value_id)id == METACALL_THROWABLE)
			{
				std::cout << metacall_value_id_name((enum metacall_value_id)id) <<  " => UNSUPPORTED" << std::endl;
				continue;
			}
			void *arg = metacall_value_create((enum metacall_value_id)id);

			if (arg == NULL)
			{
				std::cout
					<< metacall_value_id_name((enum metacall_value_id)id)
					<< " => CREATION_FAILED"
					<< std::endl;

				continue;
			}

			void *args[1] = { arg };
			std::cout
				<< "Testing "
				<< id
				<< " : "
				<< metacall_value_id_name((enum metacall_value_id)id)
				<< std::endl;


			ret = metacallhv(handle, "identity", args);

			if (ret == NULL)
			{
				std::cout
					<< metacall_value_id_name((enum metacall_value_id)id)
					<< " => UNSUPPORTED"
					<< std::endl;

				metacall_value_destroy(arg);
				continue;
			}

			std::cout
				<< metacall_value_id_name((enum metacall_value_id)id)
				<< " => "
				<< metacall_value_id_name(metacall_value_id(ret))
				<< std::endl;

			metacall_value_destroy(ret);
			metacall_value_destroy(arg);
		}
	}

	metacall_destroy();
}
