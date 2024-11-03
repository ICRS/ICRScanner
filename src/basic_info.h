#ifndef BASIC_INFO_H
#define BASIC_INFO_H

namespace basic_info {
	struct UserInfo {
		bool found = false;

		bool can_print = false;
		bool inducted = false;

		String shortcode = "";
	};
} // namespace basic_info

#endif
