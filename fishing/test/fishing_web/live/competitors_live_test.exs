defmodule FishingWeb.CompetitorsLiveTest do
  use FishingWeb.ConnCase

  import Phoenix.LiveViewTest
  import Fishing.RegistrationFixtures

  @create_attrs %{first_name: "some first_name", last_name: "some last_name", email: "some email"}
  @update_attrs %{first_name: "some updated first_name", last_name: "some updated last_name", email: "some updated email"}
  @invalid_attrs %{first_name: nil, last_name: nil, email: nil}

  defp create_competitors(_) do
    competitors = competitors_fixture()
    %{competitors: competitors}
  end

  describe "Index" do
    setup [:create_competitors]

    test "lists all competitor", %{conn: conn, competitors: competitors} do
      {:ok, _index_live, html} = live(conn, ~p"/competitor")

      assert html =~ "Listing Competitor"
      assert html =~ competitors.first_name
    end

    test "saves new competitors", %{conn: conn} do
      {:ok, index_live, _html} = live(conn, ~p"/competitor")

      assert index_live |> element("a", "New Competitors") |> render_click() =~
               "New Competitors"

      assert_patch(index_live, ~p"/competitor/new")

      assert index_live
             |> form("#competitors-form", competitors: @invalid_attrs)
             |> render_change() =~ "can&#39;t be blank"

      assert index_live
             |> form("#competitors-form", competitors: @create_attrs)
             |> render_submit()

      assert_patch(index_live, ~p"/competitor")

      html = render(index_live)
      assert html =~ "Competitors created successfully"
      assert html =~ "some first_name"
    end

    test "updates competitors in listing", %{conn: conn, competitors: competitors} do
      {:ok, index_live, _html} = live(conn, ~p"/competitor")

      assert index_live |> element("#competitor-#{competitors.id} a", "Edit") |> render_click() =~
               "Edit Competitors"

      assert_patch(index_live, ~p"/competitor/#{competitors}/edit")

      assert index_live
             |> form("#competitors-form", competitors: @invalid_attrs)
             |> render_change() =~ "can&#39;t be blank"

      assert index_live
             |> form("#competitors-form", competitors: @update_attrs)
             |> render_submit()

      assert_patch(index_live, ~p"/competitor")

      html = render(index_live)
      assert html =~ "Competitors updated successfully"
      assert html =~ "some updated first_name"
    end

    test "deletes competitors in listing", %{conn: conn, competitors: competitors} do
      {:ok, index_live, _html} = live(conn, ~p"/competitor")

      assert index_live |> element("#competitor-#{competitors.id} a", "Delete") |> render_click()
      refute has_element?(index_live, "#competitor-#{competitors.id}")
    end
  end

  describe "Show" do
    setup [:create_competitors]

    test "displays competitors", %{conn: conn, competitors: competitors} do
      {:ok, _show_live, html} = live(conn, ~p"/competitor/#{competitors}")

      assert html =~ "Show Competitors"
      assert html =~ competitors.first_name
    end

    test "updates competitors within modal", %{conn: conn, competitors: competitors} do
      {:ok, show_live, _html} = live(conn, ~p"/competitor/#{competitors}")

      assert show_live |> element("a", "Edit") |> render_click() =~
               "Edit Competitors"

      assert_patch(show_live, ~p"/competitor/#{competitors}/show/edit")

      assert show_live
             |> form("#competitors-form", competitors: @invalid_attrs)
             |> render_change() =~ "can&#39;t be blank"

      assert show_live
             |> form("#competitors-form", competitors: @update_attrs)
             |> render_submit()

      assert_patch(show_live, ~p"/competitor/#{competitors}")

      html = render(show_live)
      assert html =~ "Competitors updated successfully"
      assert html =~ "some updated first_name"
    end
  end
end
