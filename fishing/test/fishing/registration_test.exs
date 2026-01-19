defmodule Fishing.RegistrationTest do
  use Fishing.DataCase

  alias Fishing.Registration

  describe "competitor" do
    alias Fishing.Registration.Competitors

    import Fishing.RegistrationFixtures

    @invalid_attrs %{first_name: nil, last_name: nil, email: nil}

    test "list_competitor/0 returns all competitor" do
      competitors = competitors_fixture()
      assert Registration.list_competitor() == [competitors]
    end

    test "get_competitors!/1 returns the competitors with given id" do
      competitors = competitors_fixture()
      assert Registration.get_competitors!(competitors.id) == competitors
    end

    test "create_competitors/1 with valid data creates a competitors" do
      valid_attrs = %{first_name: "some first_name", last_name: "some last_name", email: "some email"}

      assert {:ok, %Competitors{} = competitors} = Registration.create_competitors(valid_attrs)
      assert competitors.first_name == "some first_name"
      assert competitors.last_name == "some last_name"
      assert competitors.email == "some email"
    end

    test "create_competitors/1 with invalid data returns error changeset" do
      assert {:error, %Ecto.Changeset{}} = Registration.create_competitors(@invalid_attrs)
    end

    test "update_competitors/2 with valid data updates the competitors" do
      competitors = competitors_fixture()
      update_attrs = %{first_name: "some updated first_name", last_name: "some updated last_name", email: "some updated email"}

      assert {:ok, %Competitors{} = competitors} = Registration.update_competitors(competitors, update_attrs)
      assert competitors.first_name == "some updated first_name"
      assert competitors.last_name == "some updated last_name"
      assert competitors.email == "some updated email"
    end

    test "update_competitors/2 with invalid data returns error changeset" do
      competitors = competitors_fixture()
      assert {:error, %Ecto.Changeset{}} = Registration.update_competitors(competitors, @invalid_attrs)
      assert competitors == Registration.get_competitors!(competitors.id)
    end

    test "delete_competitors/1 deletes the competitors" do
      competitors = competitors_fixture()
      assert {:ok, %Competitors{}} = Registration.delete_competitors(competitors)
      assert_raise Ecto.NoResultsError, fn -> Registration.get_competitors!(competitors.id) end
    end

    test "change_competitors/1 returns a competitors changeset" do
      competitors = competitors_fixture()
      assert %Ecto.Changeset{} = Registration.change_competitors(competitors)
    end
  end
end
