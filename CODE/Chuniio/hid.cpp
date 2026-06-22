#include "hid.h"

#include <string>
#include <cstdio>
#include <format>
#include <vector>
#include <optional>
#include <string_view>

#include <hidsdi.h>
#include <devguid.h>
#include <setupapi.h>

static constexpr uint16_t VID = 0x2E8A;
static constexpr uint16_t PID = 0x000A;

std::optional<std::wstring> get_chuni_device_path() {
	GUID hidclass_guid = GUID_DEVCLASS_HIDCLASS;
    const GUID* guid = &hidclass_guid;
    HidD_GetHidGuid(&hidclass_guid);

    HDEVINFO dev_info = SetupDiGetClassDevs(guid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (dev_info == INVALID_HANDLE_VALUE) {
        return std::nullopt;
    }

    std::wstring search_string = std::format(L"vid_{:04x}&", VID);

    SP_DEVICE_INTERFACE_DATA interface_data {
        .cbSize = sizeof(SP_DEVICE_INTERFACE_DATA)
    };

    std::vector<std::byte> buf;

    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(dev_info, nullptr, guid, i, &interface_data); ++i) {
        DWORD size = 0;

        /* This errors even if it works so just ignore it */
        SetupDiGetDeviceInterfaceDetail(dev_info, &interface_data, nullptr, 0, &size, nullptr);

        if (size > buf.size()) {
            buf.resize(size);
        }

        std::fill_n(buf.begin(), size, std::byte{});

        auto detail_data = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(buf.data());
        detail_data->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

        if (!SetupDiGetDeviceInterfaceDetail(dev_info, &interface_data, detail_data, size, &size, nullptr)) {
            continue;
        }

        if (std::wstring_view(detail_data->DevicePath).contains(search_string)) {
            SetupDiDestroyDeviceInfoList(dev_info);
            return detail_data->DevicePath;
        }
    }

    SetupDiDestroyDeviceInfoList(dev_info);
    return std::nullopt;
}

chu_hid::~chu_hid() {
	CloseHandle(_file);
}

bool chu_hid::init() {
    GUID hidclass_guid = GUID_DEVCLASS_HIDCLASS;

    auto path = get_chuni_device_path();
    if (!path) {
        return false;
    }

	_file = CreateFile(
        path.value().c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (_file == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

bool chu_hid::read_report(report_data& res) {
    DWORD read = 0;
    
    if (!ReadFile(_file, &res, sizeof(report_data), &read, nullptr)) {
        return false;
    }

    return read == sizeof(report_data);
}