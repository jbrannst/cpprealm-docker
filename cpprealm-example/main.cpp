#include <iostream>
#include <uv.h>
#include <cpprealm/sdk.hpp>

typedef int64_t identifier_t;
typedef int64_t load_t;
typedef std::vector<double> position_t;
typedef std::chrono::time_point<std::chrono::system_clock> timestamp_t;

struct IoTObject : realm::object<IoTObject> {
    // db internal
    realm::persisted <realm::object_id> _id{realm::object_id::generate()};
    realm::persisted <identifier_t> owner_id;   // Id of the site, for multitenancy

    // user defined
    realm::persisted <identifier_t> machine_id;   // Id of the current machine_id, e.g. 1.
    realm::persisted <std::optional<identifier_t>> cycle_id;     // Id of the current production cycle, e.g. 1.
    realm::persisted <std::optional<timestamp_t>> cycle_start;   // The starting time of the current production cycle. This information will be in epoch time format.
    realm::persisted <std::optional<timestamp_t>> cycle_end;     // The ending time of the current production cycle. This information will be in epoch time format.
    realm::persisted <std::optional<load_t>> pay_load;           // The transported load in kg for the current production cycle, e.g. 17000 .
    realm::persisted <std::optional<std::string>> material_type; // The material of the load, e.g. sand.
    realm::persisted <position_t> dumping_spot;   // The longitude, latitude and altitude of the dumping spot.

    static constexpr auto schema = realm::schema(
            "IoTObject",
            realm::property<&IoTObject::_id, true>("_id"),
            realm::property<&IoTObject::owner_id>("owner_id"),

            realm::property<&IoTObject::machine_id>("machine_id"),
            realm::property<&IoTObject::cycle_id>("cycle_id"),
            realm::property<&IoTObject::cycle_start>("cycle_start"),
            realm::property<&IoTObject::cycle_end>("cycle_end"),
            realm::property<&IoTObject::pay_load>("pay_load"),
            realm::property<&IoTObject::material_type>("material_type"),
            realm::property<&IoTObject::dumping_spot>("dumping_spot"));
};

static std::vector<std::string> materials = {"sand", "cement", "lime", "mortar", "clay"};

IoTObject randomized_object() {
    int rand_index = rand() % 5;
    double rand_position_x = rand() % 5;
    double rand_position_y = rand() % 5;

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();

    IoTObject m;
    m.owner_id = 123;
    m.material_type = materials[rand_index];
    m.machine_id = 1;
    m.pay_load = round(rand_index * 2000);
    m.cycle_start = timestamp_t(start);
    m.cycle_end = timestamp_t(end);
    m.dumping_spot = position_t{rand_position_x, rand_position_y};
    return m;
}

int main() {
    std::string app_id = "<APPID>";

    // Create a new Device Sync App and turn development mode on under sync settings.
    auto app = realm::App(app_id);
    auto user = app.login(realm::App::credentials::anonymous()).get_future().get();
    auto config = user.flexible_sync_configuration();
    auto realm = realm::open<IoTObject>(std::move(config));
    realm.subscriptions().update([](realm::mutable_sync_subscription_set &subs) {
        if (!subs.find("site_one")) {
            subs.add<IoTObject>("site_one", [](auto& obj) {
                return obj.owner_id == 123;
            });
        }
    }).get_future().get();

    for (size_t count = 0; count < 100000; count++) {
        auto o = randomized_object();
        realm.write([&]() {
            realm.add(o);
        });
        std::cout << "inserted object with id: " << (*o._id).to_string() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Optional, but allow all changes to be uploaded before process terminates
    realm.get_sync_session()->wait_for_upload_completion().get_future().get();
    return 0;
}