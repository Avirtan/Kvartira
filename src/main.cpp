#include "../include/core/di/DI.hpp"
#include "../include/core/render/Render.h"
#include "../include/managers/GameManager.h"
#include "ecs/Component.h"
#include "ecs/Entity.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <memory>
#include <ostream>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

namespace di = boost::di;

class ctor {
public:
  ctor(int i) : i(i) { std::cout << "create" << std::endl; }
  int i;
  ~ctor() { std::cout << "delete " << i << std::endl; }
};

struct aggregate {
  double d;
};

class example {
public:
  std::shared_ptr<int> b;
  std::shared_ptr<example> ex;
  example() : b(nullptr), ex(nullptr){};

  ~example() {
    if (ex != nullptr) {
      std::cout << "destroy main " << std::endl;
    } else {
      std::cout << "destroy" << std::endl;
    }
    ex.reset();
  }
  void test1(const std::shared_ptr<example> &a) { ex = a; }

  void test(const std::shared_ptr<int> &a) { b = a; }

  void print() { std::cout << b.use_count() << std::endl; }
};

class ex1 {
public:
  std::unique_ptr<ex1> ex;
  std::unique_ptr<ex1 *> ex3;
  std::vector<std::unique_ptr<ex1>> ex4;
  ex1 *e;
  int i;

  ex1(int a) : ex(nullptr) { i = a; }

  ~ex1() { std::cout << "DELETe" << i << std::endl; }

  ex1(ex1 &&obj) { std::cout << "MOVE" << std::endl; }

  void setEx1(ex1 *e1) { e = e1; }

  void setEx(std::unique_ptr<ex1> &&e1) { ex4.push_back(std::move(e1)); }
  void setEx2(std::unique_ptr<ex1 *> &&e1) { ex3 = std::move(e1); }
};

std::vector<int> mi;

struct Position : Ecs::Component {
  Position(int x, int y, int z) : X(x), Y(y), Z(z) {}
  int X;
  int Y;
  int Z;
};

struct Position2 : Ecs::Component {
  Position2(int x, int y) : X(x), Y(y) {}
  int X;
  int Y;
};

class MoveSystem : public Ecs::System {
public:
  MoveSystem() {}
  ~MoveSystem() {}

  Ecs::Entity ent;

  void Init() override {
    ent = m_World->CreateEntity();
    auto p = std::make_shared<Position>(Position(1, 2, 3));
    m_World->AddComponent<Position>(ent, p);
  }

  void Update(float t) override {
    auto p = m_World->GetComponent<Position>(ent);
    auto alive = m_World->IsAliveEntity(ent);
    std::cout << "жива " << alive << std::endl;
    if (p == nullptr) {
      return;
    }
    std::cout << p->X << std::endl;
    p->X += 1;
    if (p->X > 2 && alive) {
      m_World->DropEntity(ent);
      // return;
    }
  }
};

int main() {
  // std::shared_ptr<Ecs::World> w = std::make_shared<Ecs::World>();
  // auto ent = w->CreateEntity();
  // auto ent1 = w->CreateEntity();
  // auto ent2 = w->CreateEntity();
  // auto p = std::make_shared<Position>(Position(1, 2, 3));
  // auto p1 = std::make_shared<Position>(Position(3, 4, 5));
  // w->AddComponent<Position>(ent1, p);
  // w->AddComponent<Position>(ent, p);
  // auto p3 = std::make_shared<Position>(Position(3, 4, 5));
  // w->AddComponent<Position>(ent2, p);
  // auto p2 = std::make_shared<Position2>(Position2(3, 4));
  // w->AddComponent<Position2>(ent2, p2);
  // auto p4 = std::make_shared<Position2>(Position2(3, 4));
  // w->AddComponent<Position2>(ent1, p2);

  // auto pool = w->GetPoolComponent<Position>();
  // for (auto e : pool) {
  //   // std::cout << e << std::endl;
  // }
  // // w->DropEntity(ent1);
  // auto pool1 = w->GetPoolComponent<Position, Position2>();
  // for (auto e : pool1) {
  //   std::cout << e << std::endl;
  // }

  // auto p2 = std::make_shared<Position>(Position(1, 2, 3));
  //   w->AddComponent<Position>(ent, p);

  // std::shared_ptr<MoveSystem> s = std::make_shared<MoveSystem>();
  // w->AddSystem<MoveSystem>(s);
  // w->InitSystem();
  // for (int i = 0; i < 3; i++) {
  //   w->UpdateSystem();
  // }
  // auto entity = w->CreateEntity();
  // std::cout << entity.use_count() << std::endl;
  // std::cout << (entity->GetStatus() == Ecs::StatusEntity::live) << std::endl;
  // auto entity2 = w->CreateEntity();
  // auto entity3 = w->CreateEntity();
  // std::cout << entity2 << std::endl;
  // w->DropEntity(entity2);
  // std::cout << entity2 << std::endl;
  // {
  //   auto p = std::make_shared<Position>(Position(1, 2, 3));
  //   w->AddComponent<Position>(entity, p);
  //   std::cout << p.use_count() << std::endl;
  // }
  // {
  //   auto p = w->GetComponent<Position>(entity);
  //   p->X = 100;
  // }
  // {
  //   auto p = w->GetComponent<Position>(entity);
  //   std::cout << p->X << std::endl;
  // }
  // std::cout << p->X << std::endl;
  // Position* p = w->GetComponent<Position*>();

  // std::cout << p->X;
  // std::shared_ptr<Ecs::Entity> ent1 =
  //     std::make_shared<Ecs::Entity>(Ecs::Entity(1));
  // std::shared_ptr<Ecs::Entity> ent2 =
  //     std::make_shared<Ecs::Entity>(Ecs::Entity(2));
  // std::shared_ptr<Ecs::Entity> ent3 =
  //     std::make_shared<Ecs::Entity>(Ecs::Entity(3));
  // std::shared_ptr<Ecs::Entity> ent4 =
  //     std::make_shared<Ecs::Entity>(Ecs::Entity(4));
  // std::shared_ptr<Ecs::Entity> ent5 =
  //     std::make_shared<Ecs::Entity>(Ecs::Entity(5));

  // Ecs::Entity ent1 = 1;

  std::unordered_map<Ecs::Entity, std::shared_ptr<Ecs::Component>>
      m_Components{};
  // auto s = std::make_shared<Ecs::System>();

  // m_Components.emplace(ent1, std::make_shared<Position>(Position(1, 2, 3)));
  // auto e = m_Components[ent1];
  // std::cout << e.use_count() << std::endl;
  // auto p = std::make_shared<Position>(Position(3, 2, 3));
  // m_Components[ent1] = p;
  // std::cout << p.use_count() << std::endl;
  // // std::endl;
  // m_Components[ent1] = std::make_shared<Position>(Position(3, 2, 3));

  // m_Components.emplace(ent1, std::make_shared<Position>(Position(3, 2, 3)));
  // it = m_Components.find(ent1);

  // std::cout << std::static_pointer_cast<Position>(it->second)->X <<
  // std::endl;

  // Ecs::Entity ent2 = 2;
  // Ecs::Entity ent3 = 3;
  // Ecs::Entity ent4 = 4;
  // Ecs::Entity ent5 = 5;

  // const std::vector<Ecs::Entity> e{ent5, ent1, ent3, ent2};
  // const std::vector<Ecs::Entity> e1{ent4, ent1, ent3, ent2};
  // const std::vector<Ecs::Entity> e2{ent4, ent1, ent3, ent2};

  // std::vector<Ecs::Entity> e3;
  // e3.reserve(e1.size() + e.size() + e2.size());
  // e3.insert(e3.begin(), e.begin(), e.end());
  // e3.insert(e3.begin() + e.size(), e1.begin(), e1.end());
  // e3.insert(e3.begin() + e.size() + e1.size(), e2.begin(), e2.end());
  // std::sort(e3.begin(), e3.end());
  // e3.erase( unique( e3.begin(), e3.end() ), e3.end() );

  // std::set_union(e.begin(), e.end(), e1.begin(), e1.end(), e3.begin());
  // std::set_union(e2.begin(), e2.end(), e3.begin(), e3.end(), e3.begin());
  // const std::unordered_set<Ecs::Entity> uniq{e3.begin(), e3.end()};

  // for (auto i : e3) {
  //   std::cout << i << std::endl;
  // }
  // std::cout << " " << std::endl;
  // for (auto i : uniq) {
  //   std::cout << i << std::endl;
  // }
  // std::cout << " " << std::endl;

  // // std::cout << e3[5]->GetId() << std::endl;
  // for (auto i : e3) {
  //   if (i != nullptr) {
  //     std::cout << i << std::endl;
  //   }
  // }
  // std::cout << " " << std::endl;

  // for(auto i: uniq){
  // if (i != nullptr) {
  //     std::cout << i << std::endl;
  //   }
  // }

  // std::cout << *mi.end() << std::endl;
  auto gameManager = new Managers::GameManager();
  if (gameManager->Init()) {
    gameManager->Run();
  }
  // std::string_view path = "wqefewf";
  // auto last = path.find_last_of("/");
  // if (last == std::string::npos) {
  //   std::cout << "Not found";
  //   return -1;
  // }
  // auto name = path.substr(last + 1, path.size());
  // std::cout << name << std::endl;
  // std::unique_ptr<ex1> e = {std::make_unique<ex1>(12)};
  // ex1 *e2 = new ex1(2);
  // e->setEx1(e2);

  // std::unique_ptr<ex1> e1 = {std::make_unique<ex1>(52)};
  // e->setEx(std::move(e1));
  // e1.reset();
  // ex1* e2 = e1.get();
  // std::vector<std::unique_ptr<ex1>> aex;
  // aex.push_back(std::move(e));
  // std::cout << e1->i  << std::endl;
  // e->setEx(std::move(e1));
  // std::cout << (e1 == nullptr) << std::endl;
  // std::cout << e2 << std::endl;
  // //  e->ex->i = 3;
  //    std::cout << aex.data()[0]->i << std::endl;
  //   aex.pop_back();
  // std::cout << aex.data()[0] << std::endl;
  // std::cout << e->ex->i << std::endl;
  // e.reset();
  // std::cout << (e == nullptr) << std::endl;
  // aex.clear();
  // std::cout << e1->i  << std::endl;
  /*
    1002   1
    0100   2
    0010 * 0
    0001   1

    1002   1
    0101   2
    0010 * 0
    0001   1
  */
  // glm::vec4 vec(1.0f, 2.0f, 0.0f, 1.0f);
  // glm::mat4 trans = glm::mat4(1.0f);
  // trans = glm::scale(trans, glm::vec3(1.0f, 2.0f, 0.0f));
  // trans = glm::translate(trans, glm::vec3(2.0f, 1.0f, 0.0f));
  // trans = glm::transpose(trans);
  // trans = glm::scale(trans, glm::vec3(1.0f, 2.0f, 0.0f));
  // trans[0][3] += 1;
  // std::cout << trans[3][1];
  // trans[1][3] += 1;
  // vec = trans * vec;
  // vec = vec * trans;
  // std::cout << vec.x << vec.y << vec.z << vec.w << std::endl;

  //  glm::vec4 vec1(1.0f, 0.0f, 0.0f, 1.0f);
  // trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
  // vec1 = trans * vec1;
  // std::cout << vec1.x << vec1.y << vec1.z << vec1.w << std::endl;
  // Render* r {new Render()};
  // const auto injector = di::make_injector(
  //   di::bind<int>.to(2),
  //   di::bind<double>.to(87.0)
  // );
  // auto c = injector.create<ctor>();
  // auto c1 = injector.create<ctor>();
}